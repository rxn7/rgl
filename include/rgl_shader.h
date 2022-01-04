#pragma once

#include "rgl_common.h"

typedef struct rgl_shader_t {
	id_t id;
} rgl_shader_t;

extern rgl_shader_t rgl_sprite_shader;

void rgl_shader_create_primitives();
void rgl_shader_initialize(rgl_shader_t *shader, const char *vert_src, const char *frag_src);
void rgl_shader_destroy(rgl_shader_t *shader);
