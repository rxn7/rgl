#include "player.h"

void
playerSpawn(Player *player) {
	f32 hww = (f32)_rgl_width * 0.5f;
	f32 hwh = (f32)_rgl_height * 0.5f;

	player->position.x = hww;
	player->position.y = hwh;

	player->mass = PLAYER_SPAWN_RADIUS;
	player->color = RGL_RED;
}

void
playerUpdate(Player *player, f32 dt) {
	rglV2 mouse_position;
	rglGetCursorPosInWorld(&mouse_position);

	rglV2 delta_position;
	rglV2Sub(&mouse_position, &player->position, &delta_position);
	rglV2Normalize(&delta_position, &delta_position);
	rglV2Mulf(&delta_position, dt * PLAYER_MOVE_SPEED, &delta_position);

	rglV2Add(&player->position, &delta_position, &player->position);

	rglV2Lerp(&_rgl_camera->position, &player->position, dt * 10, &_rgl_camera->position);
}

void
playerDraw(Player *player) {
	rglDrawCircle(player->color, player->position, player->mass);
}
