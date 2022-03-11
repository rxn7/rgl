#include "rgl_sprite.h"
#include "rgl_shader.h"
#include "rgl.h"

u32 uniform_locations[2] = {0};

const rgl_vertex_t _vertices[] = {
	{ {1, 1},	{1,1} },
	{ {-1, 1},	{0,1} },
	{ {-1,-1},	{0,0} },
	{ {-1,-1},	{0,0} },
	{ {1,-1},	{1,0} },
	{ {1,1},	{1,1} },
};

b8 rgl_sprite_create(rgl_sprite_t *sprite, rgl_texture_t *texture) {
	if(!sprite) {
		RGL_LOG_ERROR("Can't create sprite with null pointer");
		return false;
	}

	sprite->texture = texture;
	sprite->rotation = 0;
	sprite->size = (v2){texture->width, texture->height};
	sprite->position = (v2){0,0};

	if(!rgl_vao_create(&sprite->vao, _vertices, sizeof(_vertices) / sizeof(_vertices[0]))) {
		RGL_LOG_ERROR("Failed to create VAO for a sprite");
		return false;
	}

	return true;
}

b8 rgl_sprite_destroy(rgl_sprite_t *sprite) {
	if(!sprite) {
		RGL_LOG_ERROR("Can't destroy sprite with null pointer");
		return false;
	}

	if(!rgl_vao_destroy(&sprite->vao)) {
		RGL_LOG_ERROR("Failed to destroy sprite's VAO");
		return false;
	}

	return true;
}

void rgl_sprite_render(rgl_sprite_t *sprite) {	
	if(!sprite) {
		RGL_LOG_ERROR("Can't render sprite with null pointer");
		return;
	}

	glBindTexture(GL_TEXTURE_2D, sprite->texture->id);
	glUseProgram(g_rgl_sprite_shader->id);

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

	glUniformMatrix4fv(g_rgl_sprite_shader->uniform_locations[0], 1, false, (float *)g_rgl_data->projection_matrix);
	glUniformMatrix4fv(g_rgl_sprite_shader->uniform_locations[1], 1, false, (float *)model_matrix);

	rgl_vao_render(GL_TRIANGLES, &sprite->vao);

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
