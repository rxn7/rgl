#pragma once
#include "common.h"
#include <rgl/rgl_sprite_animator.h>

typedef enum playerAnimation {
	MOVE_LEFT = 0,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	IDLE,
	PLAYER_ANIM_COUNT,
} playerAnimation;

typedef struct Player {
	rglV2 move_dir;
	rglSprite sprite;
	rglTexture texture;
	rglAnimationTexture anim_texture;
	rglSpriteAnimator animator;
	rglSpriteAnimation *animations;
	playerAnimation animation;
} Player;

void playerCreate(Player *player, const char *texture_path);
void playerDestroy(Player *player);
void playerUpdate(Player *player, f32 dt);
void playerDraw(Player *player);
void playerPlayAnimation(Player *player, playerAnimation anim);
