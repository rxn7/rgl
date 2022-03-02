#include "rgl_immediate.h"

void rgl_immediate_draw_rect(rgl_color_t col, vec2 pos, vec2 size) {
	glColor3ub(col.r, col.g, col.b);

	f32 hsx = size[0] * 0.5f;
	f32 hsy = size[1] * 0.5f;

	glBegin(GL_QUADS);
	glVertex2f(pos[0] - hsx, pos[1] - hsy);
	glVertex2f(pos[0] + hsx, pos[1] - hsy);
	glVertex2f(pos[0] + hsx, pos[1] + hsy);
	glVertex2f(pos[0] - hsx, pos[1] + hsy);
	glEnd();
}

void rgl_immediate_draw_circle(rgl_color_t col, vec2 pos, f32 radius) {
	if(radius <= 0) {
		printf("Tried to draw circle with radius <= 0!\n");
		return;
	}

	u32 segments = 2.8f * sqrtf(radius);
	if(segments < CIRCLE_MIN_SEGMENTS) segments = CIRCLE_MIN_SEGMENTS;
	else if(segments > CIRCLE_MAX_SEGMENTS) segments = CIRCLE_MAX_SEGMENTS;

	f32 theta = PI_2F / (float)segments;
	f32 c = cosf(theta);
	f32 s = sinf(theta);
	f32 t;

	float x = radius;
	float y = 0;

	glColor3ub(col.r, col.g, col.b);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2fv(pos);

	for(f32 i=0; i<=segments; ++i) {
		glVertex2f(x + pos[0], y + pos[1]);

		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
	glEnd();
}

void rgl_immediate_draw_line(rgl_color_t col, vec2 a, vec2 b, f32 width) {
	glColor3ub(col.r, col.g, col.b);
	glLineWidth(width);

	glBegin(GL_LINES);
	glVertex2fv(a);
	glVertex2fv(b);
	glEnd();
}
