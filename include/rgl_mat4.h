#ifndef __RGL_MAT4_H
#define __RGL_MAT4_H

#include "rgl_common.h"

typedef f32 rglMat4[16];
typedef struct rglV2 rglV2; /* For some reason I have to declare this again? */

void rglMat4Copy(rglMat4 src, rglMat4 dest);
void rglMat4Identity(rglMat4 mat);
void rglMat4Ortho(rglMat4 mat, f32 left, f32 right, f32 bottom, f32 top, f32 z_near, f32 z_far);

void rglMat4Mul(rglMat4 a, rglMat4 b);
void rglMat4Translate(rglMat4 mat, rglV2 *vec);
void rglMat4Scale(rglMat4 mat, rglV2 *vec);
void rglMat4Rotate(rglMat4 mat, f32 rot);

#endif /* __RGL_MAT4_H */
