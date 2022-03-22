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

b8 rglTextureCreate(rglTexture *txt, i32 w, i32 h, u8 *pixels, u8 filter);
b8 rglTextureLoadFromFile(rglTexture *txt, const char *path, u8 filter);
void rglTextureDestroy(rglTexture *txt);

#endif /* __RGL_TEXTURE_H */
