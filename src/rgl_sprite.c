#include "rgl.h"
#include "rgl_shader.h"
#include "rgl.h"
#include "rgl_transform.h"

static rglShader *_shader = 0;

const rglVertex _vertices[] = {
	{ {-1, -1},	{0,0} },
	{ { 1, -1},	{1,0} },
	{ { 1,  1},	{1,1} },
	{ {-1,  1},	{0,1} },
};
const u16 _vertices_size = sizeof(_vertices) / sizeof(_vertices[0]);

const element_t _elements[] = { 0, 1, 2, 2, 3, 0 };
const u16 _elements_size = sizeof(_elements) / sizeof(_elements[0]);

b8
rglSpriteCreate(rglSprite *sprite, rglTexture *texture) {
	RGL_ASSERT_VALID_PTR(sprite);
	RGL_ASSERT_VALID_PTR(texture);

	sprite->transform.rotation = 0;
	sprite->texture = texture;
	sprite->transform.scale = (rglV2){texture->width, texture->height};
	sprite->transform.position = (rglV2){0,0};

	if(!rglVaoCreate(&sprite->vao, _vertices, _vertices_size, _elements, _elements_size)) {
		RGL_LOG_ERROR("Failed to create VAO for a sprite");
		return false;
	}

	return true;
}

void
rglSpriteDestroy(rglSprite *sprite) {
	RGL_ASSERT_VALID_PTR(sprite);

	rglVaoDestroy(&sprite->vao);
}

void
rglSpriteRender(rglSprite *sprite) {	
	RGL_ASSERT_VALID_PTR(sprite);
	RGL_ASSERT_VALID_PTR(sprite->texture);

	glBindTexture(GL_TEXTURE_2D, sprite->texture->id);
	glUseProgram(_shader->id);

	rglMat4 model_matrix;
	rglTransformCalculateModelMatrix(model_matrix, &sprite->transform);

	glUniformMatrix4fv(_shader->uniform_locations[0], 1, false, (float *)_rgl_camera->projection);
	glUniformMatrix4fv(_shader->uniform_locations[1], 1, false, (float *)model_matrix);

	rglVaoRender(&sprite->vao, GL_TRIANGLES);

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void
rglSpriteSetUVs(rglSprite *sprite, rglV2 *uvs) {
	RGL_ASSERT_VALID_PTR(sprite);
	RGL_ASSERT_VALID_PTR(uvs);

	glBindVertexArray(sprite->vao.id);
	glBindBuffer(GL_ARRAY_BUFFER, sprite->vao.vbo);

	rglVertex vertices[] = {
		{ {-1, -1},	uvs[0] },
		{ { 1, -1},	uvs[1] },
		{ { 1,  1},	uvs[2] },
		{ {-1,  1},	uvs[3] },
	};

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(rglVertex) * 4, &vertices);

	glBindVertexArray(0);
}

void
_rglSpriteShaderCreate(void) {
	_shader = malloc(sizeof(rglShader));

	const char *frag = { 
		#include "../shaders_out/sprite_frag.glsl" 
	};

	const char *vert = { 
		#include "../shaders_out/sprite_vert.glsl" 
	};

	rglShaderCreate(_shader, vert, frag);

	_shader->uniform_locations = calloc(2, sizeof(u32));
	_shader->uniform_locations[0] = glGetUniformLocation(_shader->id, "u_Projection");
	_shader->uniform_locations[1] = glGetUniformLocation(_shader->id, "u_Model");
}

void
_rglSpriteShaderDestroy(void) {
	rglShaderDestroy(_shader);
	free(_shader);
}
