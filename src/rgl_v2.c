#include "rgl_v2.h"
#include <math.h>

void 
rglV2Copy(rglV2 *v, rglV2 *dest) {
	dest->x = v->x;
	dest->y = v->y;
}

void
rglV2Setf(rglV2 *v, f32 f) {
	v->x = f;
	v->y = f;
}

void
rglV2Zero(rglV2 *v) {
	v->x = 0;
	v->y = 0;
}

void
rglV2Add(rglV2 *a, rglV2 *b, rglV2 *dest) {
	dest->x = a->x + b->x;
	dest->y = a->y + b->y;
}

void
rglV2Addf(rglV2 *v, f32 f, rglV2 *dest) {
	dest->x = v->x + f;
	dest->y = v->y + f;
}

void
rglV2Sub(rglV2 *a, rglV2 *b, rglV2 *dest) {
	dest->x = a->x - b->x;
	dest->y = a->y - b->y;
}

void
rglV2Subf(rglV2 *v, f32 f, rglV2 *dest) {
	dest->x = v->x - f;
	dest->y = v->y - f;
}

void
rglV2Mul(rglV2 *a, rglV2 *b, rglV2 *dest) {
	dest->x = a->x * b->x;
	dest->y = a->y * b->y;
}

void
rglV2Mulf(rglV2 *v, f32 f, rglV2 *dest) {
	dest->x = v->x * f;
	dest->y = v->y * f;
}

void
rglV2Div(rglV2 *a, rglV2 *b, rglV2 *dest) {
	if(b->x != 0) dest->x = a->x / b->x;
	if(b->y != 0) dest->y = a->y / b->y;
}

void
rglV2Divf(rglV2 *v, f32 f, rglV2 *dest) {
	if(v != 0) {
		dest->x = v->x / f;
		dest->y = v->y / f;
	}
}

void
rglV2Normalize(rglV2 *v, rglV2 *dest) {
	f32 len = rglV2Length(v);

	if(len != 0) {
		dest->x /= len;
		dest->y /= len;
	}
}

f32
rglV2Length(rglV2 *a) {
	return sqrtf(a->x*a->x + a->y*a->y);
}

void 
rglV2Lerp(rglV2 *a, rglV2 *b, f32 v, rglV2 *dest) {
	dest->x = (a->x * (1.0f - v)) + (b->x * v);
	dest->y = (a->y * (1.0f - v)) + (b->y * v);
}

void
rglV2Print(rglV2 *a, b8 newline) {
	printf("[%f, %f]", a->x, a->y);

	if(newline) {
		printf("\n");
	}
}
