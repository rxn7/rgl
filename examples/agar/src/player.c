#include "player.h"

void player_spawn(player_t *player) {
	f32 hww = (f32)_rgl_data->width * 0.5f;
	f32 hwh = (f32)_rgl_data->height * 0.5f;

	player->pos.x = hww;
	player->pos.y = hwh;

	player->radius = PLAYER_SPAWN_RADIUS;
	player->color = RGL_RED;
}

void player_update(player_t *player) {
	rglV2 mouse_pos;
	rglGetCursorPos(&mouse_pos);

	rglV2 delta_pos;
	rglV2Sub(&mouse_pos, &player->pos, &delta_pos);
	rglV2Normalize(&delta_pos, &delta_pos);

	rglV2Add(&player->pos, &delta_pos, &player->pos);
}

void player_render(player_t *player) {
	rglDrawCircle(player->color, player->pos, player->radius);
}
