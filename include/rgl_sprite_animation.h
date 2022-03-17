#ifndef __RGL_SPRITE_ANIMATION_H
#define __RGL_SPRITE_ANIMATION_H

#include "rgl_common.h"
#include "rgl_texture.h"

typedef struct rglSpriteAnimation {
	rglTexture *texture;
	u16 *frames;
	u16 frame_count;
	f32 seconds_per_frame;
	v2 frame_size; /* in pixels */
} rglSpriteAnimation;

void rgl_sprite_animation_create(rglSpriteAnimation *anim, rglTexture *texture, u16 *frames, u16 frame_count, f32 seconds_per_frame, v2 frame_size);
void rgl_sprite_animation_destroy(rglSpriteAnimation *anim);

#endif /* __RGL_SPRITE_ANIMATION_H */
