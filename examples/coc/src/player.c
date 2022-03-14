#include "player.h"
#include "common.h"

static void player_movement(player_t *player, f32 dt);
static void player_render(player_t *player);

b8 player_create(player_t *player, const char *texture_path) {
	player->sprite.texture = malloc(sizeof(rgl_texture_t));
	rgl_texture_load_from_file(player->sprite.texture, texture_path, RGL_TEXTURE_FILTER_NONE);
	rgl_sprite_create(&player->sprite);

	rgl_v2_setf(&player->sprite.size, 50);
	player->sprite.position.x = (f32)g_rgl_data->width / 2.f;
	player->sprite.position.y = (f32)g_rgl_data->height / 2.f;
}

void player_destroy(player_t *player) {
	rgl_texture_destroy(player->sprite.texture);
	free(player->sprite.texture);

	rgl_sprite_destroy(&player->sprite);
}

void player_update(player_t *player, f32 dt) {
	player_movement(player, dt);

	if(player->sprite.position.x < 0) player->sprite.position.x = 0;
	else if(player->sprite.position.x > g_rgl_data->width) player->sprite.position.x = g_rgl_data->width;

	if(player->sprite.position.y < 0) player->sprite.position.y = 0;
	else if(player->sprite.position.y > g_rgl_data->height) player->sprite.position.y = g_rgl_data->height;

	player_render(player);
}

static void player_movement(player_t *player, f32 dt) {
	rgl_v2_zero(&player->move_dir);
	if(rgl_is_key_pressed(RGL_KEY_W)) player->move_dir.y--;
	if(rgl_is_key_pressed(RGL_KEY_S)) player->move_dir.y++;
	if(rgl_is_key_pressed(RGL_KEY_A)) player->move_dir.x--;
	if(rgl_is_key_pressed(RGL_KEY_D)) player->move_dir.x++;


	if(player->move_dir.x != 0 || player->move_dir.y != 0) {
		rgl_v2_normalize(&player->move_dir, &player->move_dir);
		rgl_v2_mulf(&player->move_dir, dt * PLAYER_MOVE_SPEED, &player->move_dir);
		rgl_v2_add(&player->sprite.position, &player->move_dir, &player->sprite.position);

		/* Horizontal flip */
		if(player->move_dir.x != 0) {
			player->sprite.size.x = fabs(player->sprite.size.x) * (player->move_dir.x < 0 ? -1 : 1);
		}
	}
}

static void player_render(player_t *player) {
	rgl_sprite_render(&player->sprite);
}
