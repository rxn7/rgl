#include "rgl_sprite.h"
#include "rgl_shader.h"

void rgl_sprite_initialize(rgl_sprite_t *sprite, rgl_texture_t *texture) {
	sprite->texture = texture;

	rgl_vertex_t vertices[] = {
		{ {-1.0f, -1.0f}, {0,0} },
		{ { 1.0f, -1.0f}, {1,0} },
		{ {-1.0f,  1.0f}, {0,1} },
		{ { 1.0f,  1.0f}, {1,1} },
	};

	rgl_vbo_initialize(&sprite->vbo, vertices, 4);
	rgl_transform_initialize(&sprite->transform);
}

void rgl_sprite_destroy(rgl_sprite_t *sprite) {
	rgl_vbo_destroy(&sprite->vbo);
}

void rgl_sprite_render(rgl_sprite_t *sprite) {	
	glBindTexture(GL_TEXTURE_2D, sprite->texture->id);
	glUseProgram(rgl_sprite_shader.id);

	mat4 trans;
	rgl_transform_calculate(trans, &sprite->transform);

	s32 trans_location = glGetUniformLocation(rgl_sprite_shader.id, "transform"); 
	glUniformMatrix4fv(trans_location, 1, GL_FALSE, (const f32 *)trans);

	rgl_vbo_render(GL_TRIANGLE_STRIP, &sprite->vbo);

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
