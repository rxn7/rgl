#include "rgl_math.h"

i32
rglMathGcd_i32(i32 a, i32 b) {
	if(a < b) {
		rglSwap(&a, &b);
	}

	while(b != 0) {
		i32 mod = a%b;
		a = b;
		b = mod;
	}

	return a;
}
