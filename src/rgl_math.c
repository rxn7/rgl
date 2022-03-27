#include "rgl_math.h"

f32 rgl_math_lerp(f32 a, f32 b, f32 v) {
	return (a * (1.0f - v)) + (b * v);
}
