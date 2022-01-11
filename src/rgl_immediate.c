#include "rgl_immediate.h"

void rgl_immediate_draw_rect(rgl_color_t col, vec2 pos, vec2 size) {
	glColor3ub(col.r, col.g, col.b);
	glBegin(GL_QUADS);
		glVertex2f(pos[0] - size[0], pos[1] - size[1]);
		glVertex2f(pos[0] + size[0], pos[1] - size[1]);
		glVertex2f(pos[0] + size[0], pos[1] + size[1]);
		glVertex2f(pos[0] - size[0], pos[1] + size[1]);
	glEnd();
}
