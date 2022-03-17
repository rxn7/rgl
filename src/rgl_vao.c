#include "rgl_vao.h"

b8 rgl_vao_create(rglVao *vao, const rglVertex *vertices, u32 vertex_count) {
	RGL_ASSERT_VALID_PTR(vao);
	RGL_ASSERT_VALID_PTR(vertices);

	vao->vertex_count = vertex_count;

	glGenVertexArrays(1, &vao->id);
	glBindVertexArray(vao->id);
	
	glGenBuffers(1, &vao->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vao->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 4 * vertex_count, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(rglVertex), (void *)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(rglVertex), (void *)(sizeof(f32) * 2));

	glBindVertexArray(0);

	return true;
}

void rgl_vao_destroy(rglVao *vao) {
	RGL_ASSERT_VALID_PTR(vao);

	glDeleteBuffers(1, &vao->vbo);
	glDeleteVertexArrays(1, &vao->id);
}

void rgl_vao_render(rglVao *vao, u32 mode) {
	RGL_ASSERT_VALID_PTR(vao);

	glBindVertexArray(vao->id);
	glDrawArrays(mode, 0, vao->vertex_count);
	glBindVertexArray(0);
}
