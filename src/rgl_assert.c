#include "rgl_assert.h"
#include "rgl.h"
#include <stdarg.h>

void
_rglAssert(const char *file, i32 line, b8 expr, const char *expr_str, const char *fmsg, ...) {
	if(!expr) {
		printf(ANSII_BOLD_RED "[RGL ASSERT FAILED] Expr: %s  |  Msg: ", expr_str);

		/* Print the formatted msg */
		va_list vargs;
		va_start(vargs, fmsg);
		vprintf(fmsg, vargs);
		va_end(vargs);

		printf("  |  File: %s  |  Line: %i" ANSII_WHITE "\n", file, line);

		rglQuit();
	}
}
