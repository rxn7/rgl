/* Settings */
#include "common.h"
#include "player.h"
#include <rgl/rgl.h> 

void app_init();
void app_init();
void app_quit();
void app_update(f32 dt);

player_t player;

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
	player_create(&player, PLAYER_SPRITE_PATH);
}

void app_quit() {
	player_destroy(&player);
}

void app_update(f32 dt) {
	player_update(&player, dt);
}
