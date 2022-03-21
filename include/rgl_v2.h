#ifndef __RGL_V2_H
#define __RGL_V2_H

#include "rgl_common.h"

typedef struct rglV2 {
	f32 x, y;
} rglV2;

void rglV2Copy(rglV2 *v, rglV2 *dest);
void rglV2Setf(rglV2 *v, f32 f);

void rglV2Zero(rglV2 *v);

void rglV2Add(rglV2 *a, rglV2 *b, rglV2 *dest);
void rglV2Addf(rglV2 *a, f32 f, rglV2 *dest);

void rglV2Sub(rglV2 *a, rglV2 *b, rglV2 *dest);
void rglV2Subf(rglV2 *v, f32 f, rglV2 *dest);

void rglV2Mul(rglV2 *a, rglV2 *b, rglV2 *dest);
void rglV2Mulf(rglV2 *v, f32 f, rglV2 *dest);

void rglV2Div(rglV2 *a, rglV2 *b, rglV2 *dest);
void rglV2Divf(rglV2 *v, f32 f, rglV2 *dest);

void rglV2Normalize(rglV2 *v, rglV2 *dest);
f32 rglV2Length(rglV2 *a);

void rglV2Print(rglV2 *a, b8 newline);

void rglV2Lerp(rglV2 *a, rglV2 *b, f32 v, rglV2 *dest);

#endif /* __RGL_V2_H */
