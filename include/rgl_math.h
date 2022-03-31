#ifndef __RGL_MATH_H
#define __RGL_MATH_H

#include "rgl_common.h"

f32 rglMathLerpf(f32 a, f32 b, f32 v);
f32 rglMathClampf(f32 val, f32 min, f32 max);
i32 rglMathGcd(i32 a, i32 b); /* Greates common divisor */

#endif /* __RGL_MATH_H */
