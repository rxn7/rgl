#pragma once
#include "common.h"

typedef struct player_t {
	rglSprite sprite;
	rglV2 move_dir;
} player_t;

void player_create(player_t *player, const char *texture_path);
void player_destroy(player_t *player);
void player_update(player_t *player, f32 dt);
void player_draw(player_t *player);
