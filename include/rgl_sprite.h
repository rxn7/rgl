#pragma once

#include "rgl_vao.h"
#include "rgl_texture.h"

typedef struct rgl_sprite_t {
	v2 position;
	v2 size;
	f32 rotation;	

	rgl_vao_t vao;
	rgl_texture_t *texture;
} rgl_sprite_t;

b8 rgl_sprite_create(rgl_sprite_t *sprite, rgl_texture_t *texture);
b8 rgl_sprite_destroy(rgl_sprite_t *sprite);
void rgl_sprite_render(rgl_sprite_t *sprite);

void rgl_sprite_shader_create();
void rgl_sprite_shader_destroy();
