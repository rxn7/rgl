#include "rgl_vbo.h"

void rgl_vbo_initialize(rgl_vbo_t *vbo, rgl_vertex_t *vertices, u32 vertex_count) {
	vbo->vertex_count = vertex_count;

	glGenVertexArrays(1, &vbo->vao);
	glBindVertexArray(vbo->vao);
	
	glGenBuffers(1, &vbo->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 4 * vertex_count, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 4, (void *)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 4, (void *)(sizeof(f32) * 2));

	glBindVertexArray(0);
}

void rgl_vbo_destroy(rgl_vbo_t *vbo) {
	glDeleteBuffers(1, &vbo->vbo);
	glDeleteVertexArrays(1, &vbo->vao);
}

void rgl_vbo_render(GLenum mode, rgl_vbo_t *vbo) {
	glBindVertexArray(vbo->vao);
	glDrawArrays(mode, 0, vbo->vertex_count);
	glBindVertexArray(0);
}
