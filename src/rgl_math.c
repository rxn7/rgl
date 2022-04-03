#include "rgl_math.h"

f32
rglMathLerp_f32(f32 a, f32 b, f32 v) {
	return (a * (1.0f - v)) + (b * v);
}

f32
rglMathClamp_f32(f32 val, f32 min, f32 max) {
	if(val < min) {
		val = min;
	} else if(val > max) {
		val = max;
	}

	return val;
}

i32
rglMathGcd_i32(i32 a, i32 b) {
	if(a < b) {
		rglSwap(&a, &b);
	}

	while(b != 0) {
		u32 mod = a%b;
		a = b;
		b = mod;
	}

	return a;
}
