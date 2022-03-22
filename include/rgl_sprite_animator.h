#ifndef __RGL_SPRITE_ANIMATOR_H
#define __RGL_SPRITE_ANIMATOR_H

#include "rgl_common.h"
#include "rgl_sprite.h"
#include "rgl_sprite_animation.h"

typedef struct rglSpriteAnimator {
	rglSprite *target;
	rglSpriteAnimation *current_anim, *default_anim;
	f32 time_elapsed;
	b8 playing;
} rglSpriteAnimator;

void rglSpriteAnimatorCreate(rglSpriteAnimator *animator, rglSprite *target, rglSpriteAnimation *default_anim);
void rglSpriteAnimatorDestroy(rglSpriteAnimator *animator);
void rglSpriteAnimatorPlayNow(rglSpriteAnimator *animator, rglSpriteAnimation *anim, b8 force);
void rglSpriteAnimatorStop(rglSpriteAnimator *animator);
void rglSpriteAnimatorUpdate(rglSpriteAnimator *animator, f32 dt);

#endif /* __RGL_SPRITE_ANIMATOR_H */
