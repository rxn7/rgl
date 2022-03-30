#include "rgl_math.h"

f32 rgl_math_lerpf(f32 a, f32 b, f32 v) {
	return (a * (1.0f - v)) + (b * v);
}

f32 rgl_math_clampf(f32 val, f32 min, f32 max) {
	if(val < min) {
		val = min;
	} else if(val > max) {
		val = max;
	}
	
	return val;
}
