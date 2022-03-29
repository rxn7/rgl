#include "common.h"
#include "player.h"
#include "coin.h"

void app_init();
void app_init();
void app_quit();
void app_update(f32 dt);
void app_draw();
void player_coin_pickup_check();

player_t player;
coin_t coins[COIN_COUNT];
u32 score = 0;

int 
main(int argc, const char **argv) {
	rglAppDesc desc = (rglAppDesc) {
		.width = 1280,
		.height = 720,
		.title = "RGL | Coc",
		.background_color = RGL_WHITE,
		.update_f = app_update,
		.draw_f = app_draw,
		.init_f = app_init,
		.quit_f = app_quit,
	};

	rglStart(&desc);
}

void
app_init() {
	player_create(&player, PLAYER_TEXTURE_PATH);
	coins_initialize();

	for(u32 i=0; i<COIN_COUNT; ++i) {
		coin_respawn(&coins[i]);
	}
}

void
app_quit() {
	player_destroy(&player);
}

void
app_update(f32 dt) {
	coins_update(dt);
	player_update(&player, dt);
	player_coin_pickup_check();

	rglV2Lerp(&_rgl_data->camera->position, &player.sprite.position, dt * 10, &_rgl_data->camera->position);

	_rgl_data->camera->zoom += _rgl_data->scroll_value * 0.05f;
}

void 
app_draw() {
	if(rglIsKeyPressed(RGL_KEY_SPACE)) {
		rglDrawCircle(RGL_RED, player.sprite.position, PLAYER_COIN_PICKUP_DISTANCE);
	}

	for(u32 i=0; i<COIN_COUNT; ++i) {
		rglDrawLine(RGL_GREEN, player.sprite.position, coins[i].pos, 3);
		coin_draw(&coins[i]);
	}

	player_draw(&player);
}

void
player_coin_pickup_check() {
	rglV2 delta_pos;

	for(u32 i=0; i<COIN_COUNT; ++i) {
		rglV2Sub(&coins[i].pos, &player.sprite.position, &delta_pos);
		f32 dist = rglV2Length(&delta_pos);

		if(dist <= PLAYER_COIN_PICKUP_DISTANCE) {
			coin_respawn(&coins[i]);
			coin_play_pickup_sound();

			/* TODO: UI Text display */
			printf("Score: %u\n", ++score);
		}
	}
}
