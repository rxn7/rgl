#include "rgl_immediate.h"

void rgl_immediate_draw_rect(rgl_color_t col, v2 pos, v2 size) {
	glColor3ub(col.r, col.g, col.b);

	f32 hsx = size.x * 0.5f;
	f32 hsy = size.y * 0.5f;

	glBegin(GL_QUADS);
	glVertex2f(pos.x - hsx, pos.y - hsy);
	glVertex2f(pos.x + hsx, pos.y - hsy);
	glVertex2f(pos.x + hsx, pos.y + hsy);
	glVertex2f(pos.x - hsx, pos.y + hsy);
	glEnd();
}

void rgl_immediate_draw_circle(rgl_color_t col, v2 pos, f32 radius) {
	if(radius <= 0) {
		printf("Tried to draw circle with radius <= 0!\n");
		return;
	}

	u32 segments = 3.f * sqrtf(radius);
	if(segments < CIRCLE_MIN_SEGMENTS) segments = CIRCLE_MIN_SEGMENTS;
	else if(segments > CIRCLE_MAX_SEGMENTS) segments = CIRCLE_MAX_SEGMENTS;

	f32 theta = PI_2F / (float)segments;
	f32 c = cosf(theta);
	f32 s = sinf(theta);
	f32 t;

	f32 x = radius;
	f32 y = 0;

	glColor3ub(col.r, col.g, col.b);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(pos.x, pos.y);

	for(f32 i=0; i<=segments; ++i) {
		glVertex2f(x + pos.x, y + pos.y);

		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
	glEnd();
}

void rgl_immediate_draw_circle_outline(rgl_color_t col, v2 pos, f32 radius, f32 line_width) {
	if(radius <= 0) {
		printf("Tried to draw circle with radius <= 0!\n");
		return;
	}

	glLineWidth(line_width);

	u32 segments = 3.f * sqrtf(radius);
	if(segments < CIRCLE_MIN_SEGMENTS) segments = CIRCLE_MIN_SEGMENTS;
	else if(segments > CIRCLE_MAX_SEGMENTS) segments = CIRCLE_MAX_SEGMENTS;

	f32 theta = PI_2F / (float)segments;
	f32 c = cosf(theta);
	f32 s = sinf(theta);
	f32 t;

	f32 x = radius;
	f32 y = 0;

	glColor3ub(col.r, col.g, col.b);

	glBegin(GL_LINE_LOOP);
	for(f32 i=0; i<=segments; ++i) {
		glVertex2f(x + pos.x, y + pos.y);

		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}
	glEnd();
}

void rgl_immediate_draw_line(rgl_color_t col, v2 a, v2 b, f32 width) {
	glColor3ub(col.r, col.g, col.b);
	glLineWidth(width);

	glBegin(GL_LINES);
	glVertex2f(a.x, a.y);
	glVertex2f(b.x, b.y);
	glEnd();
}
