#include "rgl.h"
#include "rgl_shader.h"
#include "rgl.h"

static rglShader *_shader = 0;

const rglVertex _vertices[] = {
	{ {-1, -1},	{0,0} },
	{ { 1, -1},	{1,0} },
	{ { 1,  1},	{1,1} },
	{ {-1,  1},	{0,1} },
};

b8
rglSpriteCreate(rglSprite *sprite, rglTexture *texture) {
	RGL_ASSERT_VALID_PTR(sprite);
	RGL_ASSERT_VALID_PTR(texture);

	sprite->rotation = 0;
	sprite->texture = texture;
	sprite->size = (rglV2){texture->width, texture->height};
	sprite->position = (rglV2){0,0};

	if(!rglVaoCreate(&sprite->vao, _vertices, sizeof(_vertices) / sizeof(_vertices[0]))) {
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

	rglMat4 translation_matrix;
	rglMat4Identity(translation_matrix);
	rglMat4Translate(translation_matrix, &sprite->position);

	rglMat4 rotation_matrix;
	rglMat4Identity(rotation_matrix);
	rglMat4Rotate(rotation_matrix, sprite->rotation);

	rglMat4 scale_matrix;
	rglMat4Identity(scale_matrix);
	rglMat4Scale(scale_matrix, &sprite->size);

	rglMat4 model_matrix;
	rglMat4Identity(model_matrix);
	rglMat4Mul(model_matrix, translation_matrix);
	rglMat4Mul(model_matrix, rotation_matrix);
	rglMat4Mul(model_matrix, scale_matrix);

	glUniformMatrix4fv(_shader->uniform_locations[0], 1, false, (float *)_rgl_camera->projection);
	glUniformMatrix4fv(_shader->uniform_locations[1], 1, false, (float *)model_matrix);

	rglVaoRender(&sprite->vao, GL_QUADS);

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
		#include "../shaders/sprite_frag.glsl" 
	};

	const char *vert = { 
		#include "../shaders/sprite_vert.glsl" 
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
