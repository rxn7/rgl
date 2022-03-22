#include "rgl_sprite_animator.h"

void
rglSpriteAnimatorCreate(rglSpriteAnimator *animator, rglSpriteAnimation *default_anim) {
	RGL_ASSERT_VALID_PTR(animator);
	RGL_ASSERT_VALID_PTR(default_anim);

	animator->default_anim = default_anim;
	animator->current_anim = default_anim;
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
rglSpriteAnimatorUpdate(rglSpriteAnimator *animator, rglSprite *target, f32 dt) {
	if(!animator->current_anim) {
		rglSpriteAnimatorPlayNow(animator, animator->default_anim, true);
		return;
	}

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

	f32 x = (frame % animator->current_anim->anim_texture->cols) * animator->current_anim->anim_texture->frame_size_normalized.x;
	f32 y = (frame / animator->current_anim->anim_texture->cols) * animator->current_anim->anim_texture->frame_size_normalized.y;

	rglV2 uvs[4];
	uvs[0] = (rglV2){x,				y};
	uvs[1] = (rglV2){x + animator->current_anim->anim_texture->frame_size_normalized.x,	y};
	uvs[2] = (rglV2){x + animator->current_anim->anim_texture->frame_size_normalized.x,	y + animator->current_anim->anim_texture->frame_size_normalized.y};
	uvs[3] = (rglV2){x,				y + animator->current_anim->anim_texture->frame_size_normalized.y};

	rglSpriteSetUVs(target, uvs);
}
