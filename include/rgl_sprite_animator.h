#ifndef __RGL_SPRITE_ANIMATOR_H
#define __RGL_SPRITE_ANIMATOR_H

#include "rgl_common.h"
#include "rgl_sprite.h"
#include "rgl_sprite_animation.h"

typedef struct rglSpriteAnimator {
	rglSpriteAnimation *current_anim, *default_anim;
	f32 time_elapsed;
	b8 playing;
} rglSpriteAnimator;

void rglSpriteAnimatorCreate(rglSpriteAnimator *animator, rglSpriteAnimation *default_anim);
void rglSpriteAnimatorDestroy(rglSpriteAnimator *animator);
void rglSpriteAnimatorPlayNow(rglSpriteAnimator *animator, rglSpriteAnimation *anim, b8 force);
void rglSpriteAnimatorStop(rglSpriteAnimator *animator);
void rglSpriteAnimatorUpdate(rglSpriteAnimator *animator, rglSprite *target, f32 dt);

#endif /* __RGL_SPRITE_ANIMATOR_H */
