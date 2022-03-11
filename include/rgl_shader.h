#pragma once

#include "rgl_common.h"

typedef struct rgl_shader_t {
	id_t id;
	u32 *uniform_locations;
} rgl_shader_t;

extern rgl_shader_t *g_rgl_sprite_shader;

void rgl_shader_create_defaults();
void rgl_shader_destroy_defaults();

b8 rgl_shader_create(rgl_shader_t *shader, const char *vert_src, const char *frag_src);
b8 rgl_shader_destroy(rgl_shader_t *shader);
