#include "rgl_immediate.h"
#include "rgl.h"

static void
_rglDrawRect(u32 mode, rglColor *col, rglV2 *pos, rglV2 *size) {
	glColor3ub(col->r, col->g, col->b);

	glBegin(mode);
	glVertex2f(pos->x - size->x, pos->y - size->y);
	glVertex2f(pos->x + size->x, pos->y - size->y);
	glVertex2f(pos->x + size->x, pos->y + size->y);
	glVertex2f(pos->x - size->x, pos->y + size->y);
	glEnd();
}

void
rglDrawRect(rglColor col, rglV2 pos, rglV2 size) {
	_rglDrawRect(GL_QUADS, &col, &pos, &size);
}

void
rglDrawRectOutline(rglColor col, rglV2 pos, rglV2 size, f32 line_width) {
	glLineWidth(line_width);
	_rglDrawRect(GL_LINE_LOOP, &col, &pos, &size);
}

static void
_rglDrawCircle(u32 mode, rglColor *col, rglV2 *pos, f32 radius) {
	if(radius <= 0) {
		printf("Tried to draw circle with radius <= 0!\n");
		return;
	}

	u32 segments = 1.5f * sqrtf(radius) * _rgl_camera->zoom;
	segments = rglMathClamp_u32(segments, CIRCLE_MIN_SEGMENTS, CIRCLE_MAX_SEGMENTS);

	f32 theta = RGL_PI_2F / (float)segments;
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

void
rglDrawCircle(rglColor col, rglV2 pos, f32 radius) {
	_rglDrawCircle(GL_TRIANGLE_FAN, &col, &pos, radius);
}

void
rglDrawCircleOutline(rglColor col, rglV2 pos, f32 radius, f32 line_width) {
	glLineWidth(line_width);
	_rglDrawCircle(GL_LINE_LOOP, &col, &pos, radius);
}

void
rglDrawLine(rglColor col, rglV2 a, rglV2 b, f32 width) {
	glColor3ub(col.r, col.g, col.b);
	glLineWidth(width);

	glBegin(GL_LINES);
	glVertex2f(a.x, a.y);
	glVertex2f(b.x, b.y);
	glEnd();
}
