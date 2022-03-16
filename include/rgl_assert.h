#ifndef __RGL_ASSERT_H
#define __RGL_ASSERT_H

#include "rgl_common.h"

#ifndef RGL_NO_ASSERT
	#define RGL_ASSERT(expr, quit) if(!expr) { _rgl_assert(expr, __FILE__, __LINE__, #expr, quit); return; }
	#define RGL_ASSERT_RET_B8(expr, quit) if(!expr) { _rgl_assert(expr, __FILE__, __LINE__, #expr, quit); return false; }
#else
	#define RGL_ASSERT(...)
#endif

void _rgl_assert(b8 expr, const char *file, i32 line, const char *expr_str, b8 quit);

#endif /* __RGL_ASSERT_H */
