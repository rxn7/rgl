#include "rgl_vao.h"

b8 rgl_vao_create(rgl_vao_t *vao, const rgl_vertex_t *vertices, u32 vertex_count) {
	RGL_ASSERT_RET_B8(vao, false);
	RGL_ASSERT_RET_B8(vertices, false);

	vao->vertex_count = vertex_count;

	glGenVertexArrays(1, &vao->id);
	glBindVertexArray(vao->id);
	
	glGenBuffers(1, &vao->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vao->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 4 * vertex_count, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(rgl_vertex_t), (void *)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(rgl_vertex_t), (void *)(sizeof(f32) * 2));

	glBindVertexArray(0);

	return true;
}

void rgl_vao_destroy(rgl_vao_t *vao) {
	RGL_ASSERT(vao, false);

	glDeleteBuffers(1, &vao->vbo);
	glDeleteVertexArrays(1, &vao->id);
}

void rgl_vao_render(rgl_vao_t *vao, u32 mode) {
	RGL_ASSERT(vao, false);

	glBindVertexArray(vao->id);
	glDrawArrays(mode, 0, vao->vertex_count);
	glBindVertexArray(0);
}
