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

b8 rgl_sprite_create(rglSprite *sprite, rglTexture *texture) {
	RGL_ASSERT_VALID_PTR(sprite);
	RGL_ASSERT_VALID_PTR(texture);

	sprite->rotation = 0;
	sprite->texture = texture;
	sprite->size = (v2){texture->width, texture->height};
	sprite->position = (v2){0,0};

	if(!rgl_vao_create(&sprite->vao, _vertices, sizeof(_vertices) / sizeof(_vertices[0]))) {
		RGL_LOG_ERROR("Failed to create VAO for a sprite");
		return false;
	}

	return true;
}

void rgl_sprite_destroy(rglSprite *sprite) {
	RGL_ASSERT_VALID_PTR(sprite);

	rgl_vao_destroy(&sprite->vao);
}

void rgl_sprite_render(rglSprite *sprite) {	
	RGL_ASSERT_VALID_PTR(sprite);
	RGL_ASSERT_VALID_PTR(sprite->texture);

	glBindTexture(GL_TEXTURE_2D, sprite->texture->id);
	glUseProgram(_shader->id);

	mat4 translation_matrix;
	rgl_mat4_identity(translation_matrix);
	rgl_mat4_translate(translation_matrix, sprite->position);

	mat4 rotation_matrix;
	rgl_mat4_identity(rotation_matrix);
	rgl_mat4_rotate(rotation_matrix, sprite->rotation);

	mat4 scale_matrix;
	rgl_mat4_identity(scale_matrix);
	rgl_mat4_scale(scale_matrix, sprite->size);

	mat4 model_matrix;
	rgl_mat4_identity(model_matrix);
	rgl_mat4_mul(model_matrix, translation_matrix);
	rgl_mat4_mul(model_matrix, rotation_matrix);
	rgl_mat4_mul(model_matrix, scale_matrix);

	glUniformMatrix4fv(_shader->uniform_locations[0], 1, false, (float *)_rgl_data->projection_matrix);
	glUniformMatrix4fv(_shader->uniform_locations[1], 1, false, (float *)model_matrix);

	rgl_vao_render(&sprite->vao, GL_QUADS);

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void _rgl_sprite_shader_create(void) {
	_shader = malloc(sizeof(rglShader));

	/* Sprite shader */
	rgl_shader_create(_shader, 
		/* VERTEX SHADER */
		"#version 330 core\n" \
		"layout (location = 0) in vec2 a_Pos;\n" \
		"layout (location = 1) in vec2 a_UV;\n" \
		"uniform mat4 u_Projection;\n" \
		"uniform mat4 u_Model;\n" \
		"out vec2 UV;\n" \
		"void main() {\n" \
		"	gl_Position = u_Projection * u_Model * vec4(a_Pos, 0.0, 1.0);\n" \
		"	UV = a_UV;\n" \
		"}\n",

		/* FRAGMENT SHADER */
		"#version 330 core\n" \
		"out vec4 FragColor;\n" \
		"in vec2 UV;\n" \
		"uniform sampler2D u_Texture;\n" \
		"void main() {\n" \
		"	FragColor = texture(u_Texture, UV);\n" \
		"}\n"
	);

	_shader->uniform_locations = calloc(2, sizeof(u32));
	_shader->uniform_locations[0] = glGetUniformLocation(_shader->id, "u_Projection");
	_shader->uniform_locations[1] = glGetUniformLocation(_shader->id, "u_Model");
}

void _rgl_sprite_shader_destroy(void) {
	rgl_shader_destroy(_shader);
	free(_shader);
}
