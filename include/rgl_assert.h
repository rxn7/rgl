#ifndef __RGL_ASSERT_H
#define __RGL_ASSERT_H

#include "rgl_common.h"

#define RGL_ASSERT(expr, msg, ...) if(!(expr)) _rglAssert(__FILE__, __LINE__, expr, #expr, msg, ##__VA_ARGS__)
#define RGL_ASSERT_VALID_PTR(ptr) if(!(ptr)) _rglAssert(__FILE__, __LINE__, false, #ptr, #ptr " is not a valid pointer")

void _rglAssert(const char *file, i32 line, b8 expr, const char *expr_str, const char *fmsg, ...);

#endif /* __RGL_ASSERT_H */
