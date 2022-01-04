#pragma once

#include "rgl_common.h"
#include "rgl_vertex.h"

typedef struct rgl_vbo_t {
	id_t vao;
	id_t vbo;
	u32 vertex_count;
} rgl_vbo_t;

void rgl_vbo_initialize(rgl_vbo_t *vbo, rgl_vertex_t *vertex, u32 vertex_count);
void rgl_vbo_destroy(rgl_vbo_t *vbo);
void rgl_vbo_render(GLenum mode, rgl_vbo_t *vbo);
