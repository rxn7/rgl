#include "rgl_vao.h"

b8 rgl_vao_create(rgl_vao_t *vao, const rgl_vertex_t *vertices, u32 vertex_count) {
#ifdef RGL_DEBUG
	if(!vao) {
		RGL_LOG_ERROR("Can't create VAO with null pointer");
		return false;
	}
#endif

	vao->vertex_count = vertex_count;

	glGenVertexArrays(1, &vao->id);
	glBindVertexArray(vao->id);
	
	glGenBuffers(1, &vao->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vao->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * 4 * vertex_count, vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 4, (void *)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(f32) * 4, (void *)(sizeof(f32) * 2));

	glBindVertexArray(0);

	return true;
}

b8 rgl_vao_destroy(rgl_vao_t *vao) {
#ifdef RGL_DEBUG
	if(!vao) {
		RGL_LOG_ERROR("Can't destroy VAO with null pointer");
		return false;
	}
#endif

	glDeleteBuffers(1, &vao->vbo);
	glDeleteVertexArrays(1, &vao->id);

	return true;
}

void rgl_vao_render(u32 mode, rgl_vao_t *vao) {
#ifdef RGL_DEBUG
	if(!vao) {
		RGL_LOG_ERROR("Can't render VAO with null pointer");
		return;
	}
#endif

	glBindVertexArray(vao->id);
	glDrawArrays(mode, 0, vao->vertex_count);
	glBindVertexArray(0);
}
