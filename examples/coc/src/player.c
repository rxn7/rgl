#include "player.h"
#include "common.h"

static rglTexture _texture = {0};

static void player_movement(player_t *player, f32 dt);
static void player_render(player_t *player);

void player_create(player_t *player, const char *texture_path) {
	rglTextureLoadFromFile(&_texture, texture_path, RGL_TEXTURE_FILTER_NONE);
	rglSpriteCreate(&player->sprite, &_texture);

	rglV2Setf(&player->sprite.size, 50);
	player->sprite.position.x = (f32)_rgl_data->width / 2.f;
	player->sprite.position.y = (f32)_rgl_data->height / 2.f;
}

void player_destroy(player_t *player) {
	rglTextureDestroy(player->sprite.texture);
	rglSpriteDestroy(&player->sprite);
}

void player_update(player_t *player, f32 dt) {
	player_movement(player, dt);

	i32 ww, wh;
	rglGetWindowSize(&ww, &wh);

	if(player->sprite.position.x < 0) player->sprite.position.x = 0;
	else if(player->sprite.position.x > ww) player->sprite.position.x = ww;

	if(player->sprite.position.y < 0) player->sprite.position.y = 0;
	else if(player->sprite.position.y > wh) player->sprite.position.y = wh;

	player_render(player);
}

static void player_movement(player_t *player, f32 dt) {
	rglV2Zero(&player->move_dir);
	if(rglIsKeyPressed(RGL_KEY_W)) player->move_dir.y--;
	if(rglIsKeyPressed(RGL_KEY_S)) player->move_dir.y++;
	if(rglIsKeyPressed(RGL_KEY_A)) player->move_dir.x--;
	if(rglIsKeyPressed(RGL_KEY_D)) player->move_dir.x++;


	if(player->move_dir.x != 0 || player->move_dir.y != 0) {
		rglV2Normalize(&player->move_dir, &player->move_dir);
		rglV2Mulf(&player->move_dir, dt * PLAYER_MOVE_SPEED, &player->move_dir);
		rglV2Add(&player->sprite.position, &player->move_dir, &player->sprite.position);

		/* Horizontal flip */
		if(player->move_dir.x != 0) {
			player->sprite.size.x = fabs(player->sprite.size.x) * (player->move_dir.x < 0 ? -1 : 1);
		}
	}
}

static void player_render(player_t *player) {
	rglSpriteRender(&player->sprite);
}
