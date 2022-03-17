#include "rgl_assert.h"
#include "rgl.h"
#include <stdarg.h>

/* TODO: Format the msg text */
void _rglAssert(const char *file, i32 line, b8 expr, const char *expr_str, const char *fmsg, ...) {
	if(!expr) {
		fprintf(stderr, "[RGL ASSERT FAILED] Expr: %s  |  Msg: ", expr_str);

		/* Print the formatted msg */
		va_list vargs;
		va_start(vargs, fmsg);
		vfprintf(stderr, fmsg, vargs);
		va_end(vargs);

		fprintf(stderr, "  |  File: %s  |  Line: %i\n", file, line);

		rglQuit();
	}
}
