#ifndef __RGL_VAO_H
#define __RGL_VAO_H

#include "rgl_common.h"
#include "rgl_vertex.h"

#define element_t u16
#define RGL_ELEMENT_GL_TYPE GL_UNSIGNED_SHORT

typedef struct rglVao {
	id_t id;
	id_t vbo;
	id_t ebo;
	u16 vertex_count;
	u16 element_count;
} rglVao;

b8 rglVaoCreate(rglVao *vao, const rglVertex *vertex, u16 vertex_count, const element_t *elements, u16 element_count);
void rglVaoDestroy(rglVao *vao);
void rglVaoRender(rglVao *vao, u32 mode);

#endif /* __RGL_VAO_H */
