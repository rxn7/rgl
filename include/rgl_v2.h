#pragma once

#include "rgl_common.h"

typedef struct v2 {
	f32 x, y;
} v2;

void rgl_v2_cpy(v2 *a, v2 *dest);
void rgl_v2_setf(v2 *a, f32 v);

void rgl_v2_zero(v2 *a);

void rgl_v2_add(v2 *a, v2 *b, v2 *dest);
void rgl_v2_addf(v2 *a, f32 v, v2 *dest);

void rgl_v2_sub(v2 *a, v2 *b, v2 *dest);
void rgl_v2_subf(v2 *a, f32 v, v2 *dest);

void rgl_v2_mul(v2 *a, v2 *b, v2 *dest);
void rgl_v2_mulf(v2 *a, f32 v, v2 *dest);

void rgl_v2_div(v2 *a, v2 *b, v2 *dest);
void rgl_v2_divf(v2 *a, f32 v, v2 *dest);

void rgl_v2_normalize(v2 *a, v2 *dest);

void rgl_v2_print(v2 *a, b8 newline);

f32 rgl_v2_len(v2 *a);
