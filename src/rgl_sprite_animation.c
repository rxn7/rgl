#include "rgl_sprite_animation.h"

void rgl_sprite_animation_create(rgl_sprite_animation_t *anim, rgl_texture_t *texture, u16 *frames, u16 frame_count, f32 seconds_per_frame, v2 frame_size) {
	RGL_ASSERT_VALID_PTR(anim);
	RGL_ASSERT_VALID_PTR(texture);
	RGL_ASSERT_VALID_PTR(frames);
	RGL_ASSERT(frame_count > 0, "frame_count has to be greater than 0");

	anim->texture = texture;
	anim->frame_count = frame_count;
	anim->seconds_per_frame = seconds_per_frame;
	anim->frame_size = frame_size;

	u32 frames_sz = sizeof(u16) * frame_count;
	anim->frames = malloc(frames_sz);
	memcpy(frames, anim->frames, frames_sz);
}

void rgl_sprite_animation_destroy(rgl_sprite_animation_t *anim) {
	RGL_ASSERT_VALID_PTR(anim);
	RGL_ASSERT_VALID_PTR(anim->frames);

	free(anim->frames);
}
