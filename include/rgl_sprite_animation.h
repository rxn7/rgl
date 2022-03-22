#ifndef __RGL_SPRITE_ANIMATION_H
#define __RGL_SPRITE_ANIMATION_H

#include "rgl_common.h"
#include "rgl_texture.h"

typedef struct rglSpriteAnimation {
	rglTexture *texture;
	u16 *frames;
	u16 frame_count;
	f32 seconds_per_frame;
	rglV2 frame_size; /* in pixels */
	b8 looping;
} rglSpriteAnimation;

void rglSpriteAnimationCreate(rglSpriteAnimation *anim, rglTexture *texture, u16 *frames, u16 frame_count, f32 seconds_per_frame, rglV2 frame_size, b8 looping);
void rglSpriteAnimationDestroy(rglSpriteAnimation *anim);

#endif /* __RGL_SPRITE_ANIMATION_H */
