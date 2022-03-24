#include <rgl/rgl.h>
#include "food.h"
#include "player.h"

void app_init(void);
void app_update(f32 dt);
void app_quit(void);
void app_draw(void);
void spawn_foods(void);

#define FOOD_COUNT 300

food_t foods[FOOD_COUNT];
player_t player;

int
main(int argc, const char **argv) {
	rglAppDesc desc = (rglAppDesc){
		.title = "RGL | Agar",
		.width = 1280,
		.height = 720,
		.background_color = RGL_RGB(255,255,255),
		.init_f = app_init,
		.update_f = app_update,
		.draw_f = app_draw,
		.quit_f = app_quit,
	};

	rglStart(&desc);
}

void
app_init(void) {
	player_spawn(&player);
	spawn_foods();
}

void
app_update(f32 dt) {
	if(rglIsKeyJustPressed(RGL_KEY_R)) {
		player_spawn(&player);
		spawn_foods();
	}

	for(u32 i=0; i<FOOD_COUNT; ++i) {
		food_update(&foods[i], &player, dt);
	}

	player_update(&player, dt);
}

void app_draw(void) {
	for(u32 i=0; i<FOOD_COUNT; ++i) {
		food_draw(&foods[i]);
	}

	player_draw(&player);
}

void
app_quit(void) {
}

void
spawn_foods(void) {
	for(u32 i=0; i<FOOD_COUNT; ++i) {
		food_spawn(&foods[i], &player);
	}
}
