#pragma once

#include "rgl_common.h"

typedef struct rgl_shader_t {
	u32 id;
	u32 *uniform_locations;
} rgl_shader_t;

void rgl_shader_create_defaults();
void rgl_shader_destroy_defaults();

b8 rgl_shader_create(rgl_shader_t *shader, const char *vert_src, const char *frag_src);
b8 rgl_shader_destroy(rgl_shader_t *shader);
