#ifndef __RGL_MAT4_H
#define __RGL_MAT4_H

#include "rgl_common.h"
#include "rgl_v2.h"

typedef struct v2 v2;

typedef f32 rglMat4[16];

void rglMat4Cpy(rglMat4 src, rglMat4 dest);
void rglMat4Identity(rglMat4 mat);
void rglMat4Ortho(rglMat4 mat, f32 left, f32 right, f32 bottom, f32 top, f32 z_near, f32 z_far);

void rglMat4Mul(rglMat4 a, rglMat4 b);
void rglMat4Translate(rglMat4 mat, v2 vec);
void rglMat4Scale(rglMat4 mat, v2 vec);
void rglMat4Rotate(rglMat4 mat, f32 rot);

#endif /* __RGL_MAT4_H */
