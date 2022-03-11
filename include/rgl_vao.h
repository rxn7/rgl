#pragma once

#include "rgl_common.h"
#include "rgl_vertex.h"

typedef struct rgl_vao_t {
	id_t id;
	id_t vbo;
	u32 vertex_count;
} rgl_vao_t;

b8 rgl_vao_create(rgl_vao_t *vao, const rgl_vertex_t *vertex, u32 vertex_count);
b8 rgl_vao_destroy(rgl_vao_t *vao);
void rgl_vao_render(u32 mode, rgl_vao_t *vao);
