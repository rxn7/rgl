#ifndef __RGL_SPRITE_ANIMATION_H
#define __RGL_SPRITE_ANIMATION_H

#include "rgl_common.h"
#include "rgl_texture.h"

typedef struct rgl_sprite_animation_t {
	rgl_texture_t *texture;
	u16 *frames;
	u16 frame_count;
	f32 seconds_per_frame;
	v2 frame_size; /* in pixels */
} rgl_sprite_animation_t;

void rgl_sprite_animation_create(rgl_sprite_animation_t *anim, rgl_texture_t *texture, u16 *frames, u16 frame_count, f32 seconds_per_frame, v2 frame_size);
void rgl_sprite_animation_destroy(rgl_sprite_animation_t *anim);

#endif /* __RGL_SPRITE_ANIMATION_H */
