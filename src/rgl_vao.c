#include "rgl_vao.h"

b8 rglVaoCreate(rglVao *vao, const rglVertex *vertices, u16 vertex_count, const u16 *elements, u16 element_count) {
  RGL_ASSERT_VALID_PTR(vao);
  RGL_ASSERT_VALID_PTR(vertices);
  RGL_ASSERT_VALID_PTR(elements);

  vao->vertex_count = vertex_count;
  vao->element_count = element_count;

  glGenVertexArrays(1, &vao->id);
  glBindVertexArray(vao->id);

  glGenBuffers(1, &vao->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vao->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(rglVertex) * vertex_count, vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &vao->ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_t) * element_count, elements, GL_STATIC_DRAW);

  // POSITION
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(rglVertex), (void *)0);

  // UV
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(rglVertex), (void *)(sizeof(f32) * 2));

  glBindVertexArray(0);

  return true;
}

void rglVaoDestroy(rglVao *vao) {
  RGL_ASSERT_VALID_PTR(vao);

  glDeleteBuffers(1, &vao->vbo);
  glDeleteVertexArrays(1, &vao->id);
}

void rglVaoRender(rglVao *vao, u32 mode) {
  RGL_ASSERT_VALID_PTR(vao);

  glBindVertexArray(vao->id);
  glDrawElements(mode, vao->element_count, RGL_ELEMENT_GL_TYPE, 0);
  glBindVertexArray(0);
}
