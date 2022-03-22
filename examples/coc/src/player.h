#pragma once
#include "common.h"
#include <rgl/rgl_sprite_animator.h>

typedef enum player_animation_t {
	MOVE_LEFT = 0,
	MOVE_RIGHT,
	MOVE_UP,
	MOVE_DOWN,
	IDLE,
	PLAYER_ANIM_COUNT,
} player_animation_t;

typedef struct player_t {
	rglV2 move_dir;
	rglSprite sprite;
	rglTexture texture;
	rglAnimationTexture anim_texture;
	rglSpriteAnimator animator;
	rglSpriteAnimation *animations;
	player_animation_t animation;
} player_t;

void player_create(player_t *player, const char *texture_path);
void player_destroy(player_t *player);
void player_update(player_t *player, f32 dt);
void player_draw(player_t *player);
void player_play_animation(player_t *player, player_animation_t anim);
