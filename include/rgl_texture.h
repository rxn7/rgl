#ifndef __RGL_TEXTURE_H
#define __RGL_TEXTURE_H

#include "rgl_common.h"
#include "rgl_color.h"

#define RGL_TEXTURE_FILTER_NONE 0
#define RGL_TEXTURE_FILTER_LINEAR 1

typedef struct rgl_texture_t {
	u32 id;
	i32 width, height;
} rgl_texture_t;

b8 rgl_texture_create(rgl_texture_t *txt, i32 w, i32 h, u8 *pixels, u8 filter);
b8 rgl_texture_load_from_file(rgl_texture_t *txt, const char *path, u8 filter);
void rgl_texture_destroy(rgl_texture_t *txt);

#endif /* __RGL_TEXTURE_H */
