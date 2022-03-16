#include "common.h"
#include "player.h"
#include "coin.h"

void app_init();
void app_init();
void app_quit();
void app_update(f32 dt);
void player_coin_pickup_check();

player_t player;
coin_t coin;
u32 score = 0;

int main(int argc, const char **argv) {
	rgl_app_desc_t desc = (rgl_app_desc_t) {
		.width = 640,
		.height = 480,
		.title = "RGL | Coc",
		.background_color = RGL_RGB(10, 70, 10),
		.update_f = app_update,
		.init_f = app_init,
		.quit_f = app_quit,
	};

	rgl_init(&desc);
}

void app_init() {
	player_create(&player, PLAYER_TEXTURE_PATH);

	coin_initialize();
	coin_respawn(&coin);
}

void app_quit() {
	player_destroy(&player);
}

void app_update(f32 dt) {
	player_update(&player, dt);
	player_coin_pickup_check();
	coin_render(&coin);
}

void player_coin_pickup_check() {
	v2 delta_pos;
	rgl_v2_sub(&coin.pos, &player.sprite.position, &delta_pos);

	f32 dist = rgl_v2_len(&delta_pos);

	if(dist <= PLAYER_COIN_PICKUP_DISTANCE) {
		coin_respawn(&coin);
		coin_play_pickup_sound();

		/* TODO: UI Text display */
		printf("Score: %u\n", ++score);
	}
}
