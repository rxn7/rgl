#include "rgl_v2.h"
#include <math.h>

void 
rglV2Copy(v2 *v, v2 *dest) {
	dest->x = v->x;
	dest->y = v->y;
}

void
rglV2Setf(v2 *v, f32 f) {
	v->x = f;
	v->y = f;
}

void
rglV2Zero(v2 *v) {
	v->x = 0;
	v->y = 0;
}

void
rglV2Add(v2 *a, v2 *b, v2 *dest) {
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
}

void
rglV2Addf(v2 *v, f32 f, v2 *dest) {
	dest->x = v->x + f;
	dest->y = v->y + f;
}

void
rglV2Sub(v2 *a, v2 *b, v2 *dest) {
	dest->x = a->x - b->x;
	dest->y = a->y - b->y;
}

void
rglV2Subf(v2 *v, f32 f, v2 *dest) {
	dest->x = v->x - f;
	dest->y = v->y - f;
}

void
rglV2Mul(v2 *a, v2 *b, v2 *dest) {
	dest->x = a->x * b->x;
	dest->y = a->y * b->y;
}

void
rglV2Mulf(v2 *v, f32 f, v2 *dest) {
	dest->x = v->x * f;
	dest->y = v->y * f;
}

void
rglV2Div(v2 *a, v2 *b, v2 *dest) {
	if(b->x != 0) dest->x = a->x / b->x;
	if(b->y != 0) dest->y = a->y / b->y;
}

void
rglV2Divf(v2 *v, f32 f, v2 *dest) {
	if(v != 0) {
		dest->x = v->x / f;
		dest->y = v->y / f;
	}
}

void
rglV2Normalize(v2 *v, v2 *dest) {
	f32 len = rglV2Length(v);

	if(len != 0) {
		dest->x /= len;
		dest->y /= len;
	}
}

f32
rglV2Length(v2 *a) {
	return sqrtf(a->x*a->x + a->y*a->y);
}

void
rglV2Print(v2 *a, b8 newline) {
	printf("[%f, %f]", a->x, a->y);

	if(newline) {
		printf("\n");
	}
}
