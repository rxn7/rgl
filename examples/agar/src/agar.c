#include <rgl/rgl.h>
#include "food.h"
#include "player.h"

void appInit(void);
void appUpdate(f32 dt);
void appQuit(void);
void appDraw(void);
void spawnFoods(void);

#define FOOD_COUNT 300

Food foods[FOOD_COUNT];
Player player;

int
main(int argc, const char **argv) {
	rglAppDesc desc = (rglAppDesc){
		.title = "RGL | Agar",
		.width = 1280,
		.height = 720,
		.background_color = RGL_RGB(255,255,255),
		.init_f = appInit,
		.update_f = appUpdate,
		.draw_f = appDraw,
		.quit_f = appQuit,
	};

	rglStart(&desc);
}

void
appInit(void) {
	playerSpawn(&player);
	spawnFoods();
}

void
appUpdate(f32 dt) {
	if(rglIsKeyJustPressed(RGL_KEY_R)) {
		playerSpawn(&player);
		spawnFoods();
	}

	for(u32 i=0; i<FOOD_COUNT; ++i) {
		foodUpdate(&foods[i], &player, dt);
	}

	playerUpdate(&player, dt);
}

void appDraw(void) {
	for(u32 i=0; i<FOOD_COUNT; ++i) {
		foodDraw(&foods[i]);
	}

	playerDraw(&player);
}

void
appQuit(void) {
}

void
spawnFoods(void) {
	for(u32 i=0; i<FOOD_COUNT; ++i) {
		foodSpawn(&foods[i], &player);
	}
}
