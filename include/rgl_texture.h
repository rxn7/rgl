#ifndef __RGL_TEXTURE_H
#define __RGL_TEXTURE_H

#include "rgl_common.h"
#include "rgl_color.h"

#define RGL_TEXTURE_FILTER_NONE 0
#define RGL_TEXTURE_FILTER_LINEAR 1

typedef struct rglTexture {
	u32 id;
	i32 width, height;
} rglTexture;

b8 rgl_texture_create(rglTexture *txt, i32 w, i32 h, u8 *pixels, u8 filter);
b8 rgl_texture_load_from_file(rglTexture *txt, const char *path, u8 filter);
void rgl_texture_destroy(rglTexture *txt);

#endif /* __RGL_TEXTURE_H */
