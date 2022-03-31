#include "player.h"

void
player_spawn(player_t *player) {
	f32 hww = (f32)_rgl_width * 0.5f;
	f32 hwh = (f32)_rgl_height * 0.5f;

	player->pos.x = hww;
	player->pos.y = hwh;

	player->mass = PLAYER_SPAWN_RADIUS;
	player->color = RGL_RED;
}

void
player_update(player_t *player, f32 dt) {
	rglV2 mouse_pos;
	rglGetCursorPosInWorld(&mouse_pos);

	rglV2 delta_pos;
	rglV2Sub(&mouse_pos, &player->pos, &delta_pos);
	rglV2Normalize(&delta_pos, &delta_pos);
	rglV2Mulf(&delta_pos, dt * PLAYER_MOVE_SPEED, &delta_pos);

	rglV2Add(&player->pos, &delta_pos, &player->pos);

	rglV2Lerp(&_rgl_camera->position, &player->pos, dt * 10, &_rgl_camera->position);

	_rgl_camera->zoom = rglMathLerpf(_rgl_camera->zoom, 1.f / player->mass * 100, dt * 3); 
}

void
player_draw(player_t *player) {
	rglDrawCircle(player->color, player->pos, player->mass);
}
