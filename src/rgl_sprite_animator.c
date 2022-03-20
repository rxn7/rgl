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
rglSpriteAnimatorPlay(rglSpriteAnimator *animator, rglSpriteAnimation *anim) {
	animator->time_elapsed = 0.0f;
	animator->current_anim = anim;
}

void
rglSpriteAnimatorPlayNext(rglSpriteAnimator *animator, rglSpriteAnimation *anim) {
	animator->next_anim = anim;
}

void
rglSpriteAnimatorUpdate(rglSpriteAnimator *animator, f32 dt) {
}
