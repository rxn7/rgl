#include "rgl_immediate.h"

static void _rglDrawRect(u32 mode, rglColor *col, v2 *pos, v2 *size) {
	glColor3ub(col->r, col->g, col->b);

	f32 hsx = size->x * 0.5f;
	f32 hsy = size->y * 0.5f;

	glBegin(mode);
	glVertex2f(pos->x - hsx, pos->y - hsy);
	glVertex2f(pos->x + hsx, pos->y - hsy);
	glVertex2f(pos->x + hsx, pos->y + hsy);
	glVertex2f(pos->x - hsx, pos->y + hsy);
	glEnd();
}

void rglDrawRect(rglColor col, v2 pos, v2 size) {
	_rglDrawRect(GL_QUADS, &col, &pos, &size);
}

void rglDrawRectOutline(rglColor col, v2 pos, v2 size, f32 line_width) {
	glLineWidth(line_width);
	_rglDrawRect(GL_LINE_LOOP, &col, &pos, &size);
}

static void _rglDrawCircle(u32 mode, rglColor *col, v2 *pos, f32 radius) {
	if(radius <= 0) {
		printf("Tried to draw circle with radius <= 0!\n");
		return;
	}

	u32 segments = 3.1f * sqrtf(radius);
	if(segments < CIRCLE_MIN_SEGMENTS) segments = CIRCLE_MIN_SEGMENTS;
	else if(segments > CIRCLE_MAX_SEGMENTS) segments = CIRCLE_MAX_SEGMENTS;

	f32 theta = PI_2F / (float)segments;
	f32 c = cosf(theta);
	f32 s = sinf(theta);
	f32 x = radius;
	f32 y = 0;
	f32 t;

	glColor3ub(col->r, col->g, col->b);

	glBegin(mode);

	if(mode == GL_TRIANGLE_FAN) {
		glVertex2f(pos->x, pos->y);
	}

	for(f32 i=0; i<=segments; ++i) {
		glVertex2f(x + pos->x, y + pos->y);

		t = x;
		x = c * x - s * y;
		y = s * t + c * y;
	}

	glEnd();
}

void rglDrawCircle(rglColor col, v2 pos, f32 radius) {
	_rglDrawCircle(GL_TRIANGLE_FAN, &col, &pos, radius);
}

void rglDrawCircleOutline(rglColor col, v2 pos, f32 radius, f32 line_width) {
	glLineWidth(line_width);
	_rglDrawCircle(GL_LINE_LOOP, &col, &pos, radius);
}

void rglDrawLine(rglColor col, v2 a, v2 b, f32 width) {
	glColor3ub(col.r, col.g, col.b);
	glLineWidth(width);

	glBegin(GL_LINES);
	glVertex2f(a.x, a.y);
	glVertex2f(b.x, b.y);
	glEnd();
}
