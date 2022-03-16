#include "rgl_assert.h"
#include "rgl.h"

void _rgl_assert(b8 expr, const char *file, i32 line, const char *expr_str, b8 quit) {
	printf("[RGL ASSERT] %s\tFile: %s\tLine: %i\n", expr_str, file, line);

	if(quit) {
		rgl_quit();
	}
}
