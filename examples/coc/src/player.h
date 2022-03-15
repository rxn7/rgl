#pragma once
#include "common.h"

typedef struct player_t {
	rgl_sprite_t sprite;
	v2 move_dir;
} player_t;

b8 player_create(player_t *player, const char *texture_path);
void player_destroy(player_t *player);
void player_update(player_t *player, f32 dt);