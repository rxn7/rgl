#include "rgl_sprite_animation.h"

void
rglSpriteAnimationCreate(rglSpriteAnimation *anim, rglTexture *texture, u16 *frames, u16 frame_count, f32 seconds_per_frame, rglV2 frame_size, b8 looping) {
	RGL_ASSERT_VALID_PTR(anim);
	RGL_ASSERT_VALID_PTR(texture);
	RGL_ASSERT_VALID_PTR(frames);

	anim->texture = texture;
	anim->frame_count = frame_count;
	anim->seconds_per_frame = seconds_per_frame;
	anim->frame_size = frame_size;
	anim->looping = looping;

	u32 frames_sz = sizeof(u16) * frame_count;
	anim->frames = malloc(frames_sz);
	memcpy(anim->frames, frames, frames_sz);
}

void
rglSpriteAnimationDestroy(rglSpriteAnimation *anim) {
	RGL_ASSERT_VALID_PTR(anim);
	RGL_ASSERT_VALID_PTR(anim->frames);

	free(anim->frames);
}
