#ifndef __RGL_VAO_H
#define __RGL_VAO_H

#include "rgl_common.h"
#include "rgl_vertex.h"

typedef struct rglVao {
	id_t id;
	id_t vbo;
	u32 vertex_count;
} rglVao;

b8 rgl_vao_create(rglVao *vao, const rglVertex *vertex, u32 vertex_count);
void rgl_vao_destroy(rglVao *vao);
void rgl_vao_render(rglVao *vao, u32 mode);

#endif /* __RGL_VAO_H */
