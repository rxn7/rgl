#include "rgl_sprite_animator.h"

void
rglSpriteAnimatorCreate(rglSpriteAnimator *animator, rglSprite *target, rglSpriteAnimation *default_anim) {
	RGL_ASSERT_VALID_PTR(animator);
	RGL_ASSERT_VALID_PTR(target);
	RGL_ASSERT_VALID_PTR(default_anim);

	animator->target = target;
	animator->default_anim = default_anim;
}

void
rglSpriteAnimatorDestroy(rglSpriteAnimator *animator) {
	RGL_ASSERT_VALID_PTR(animator);
}

void
rglSpriteAnimatorPlayNow(rglSpriteAnimator *animator, rglSpriteAnimation *anim, b8 force) {
	if(force || animator->current_anim != anim) {
		animator->time_elapsed = 0.0f;
		animator->current_anim = anim;
	}
}

void
rglSpriteAnimatorUpdate(rglSpriteAnimator *animator, f32 dt) {
	f32 animation_duration = animator->current_anim->frame_count * animator->current_anim->seconds_per_frame;
	animator->time_elapsed += dt;

	if(animator->time_elapsed >= animation_duration) {
		if(animator->current_anim->looping) {
			animator->time_elapsed = 0.0f;
		} else {
			rglSpriteAnimatorPlayNow(animator, animator->default_anim, true);
			return;
		}
	}

	u16 frame_idx = (u16)((animator->time_elapsed / animation_duration) * animator->current_anim->frame_count); 
	u16 frame = animator->current_anim->frames[frame_idx];
	u16 cols = animator->current_anim->texture->width / animator->current_anim->frame_size.x;

	f32 frame_size_normalized_x = animator->current_anim->frame_size.x / animator->current_anim->texture->width;
	f32 frame_size_normalized_y = animator->current_anim->frame_size.y / animator->current_anim->texture->height;

	f32 x = (frame % cols) * frame_size_normalized_x;
	f32 y = (frame / cols) * frame_size_normalized_y;

	rglV2 uvs[4];
	uvs[0] = (rglV2){x,				y};
	uvs[1] = (rglV2){x + frame_size_normalized_x,	y};
	uvs[2] = (rglV2){x + frame_size_normalized_x,	y + frame_size_normalized_y};
	uvs[3] = (rglV2){x,				y + frame_size_normalized_y};

	rglSpriteSetUVs(animator->target, uvs);
}
