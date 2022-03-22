#ifndef __RGL_ANIMATION_TEXTURE_H
#define __RGL_ANIMATION_TEXTURE_H

#include "rgl_texture.h"

typedef struct rglAnimationTexture {
	rglTexture *texture;
	u32 cols;
	rglV2 frame_size;
	rglV2 frame_size_normalized;
} rglAnimationTexture;

void rglAnimationTextureCreate(rglAnimationTexture *anim_texture, rglTexture *texture, rglV2 frame_size);
void rglAnimationTextureDestroy(rglAnimationTexture *anim_texture);

#endif /* __RGL_ANIMATION_TEXTURE_H */
