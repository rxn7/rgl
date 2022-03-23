#include <rgl/rgl.h>
#include "food.h"
#include "player.h"

void app_init(void);
void app_update(f32 dt);
void app_quit(void);
void app_draw(void);
void spawn_foods(void);

#define FOOD_COUNT 300
#define PLAYER_RADIUS_INC 0.2f
#define PLAYER_RADIUS_MAX 500

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
		rglV2 delta_pos;
		rglV2Sub(&foods[i].pos, &player.pos, &delta_pos);

		f32 dist = rglV2Length(&delta_pos);

		if(dist <= player.radius + FOOD_RADIUS) {
			if(player.radius < PLAYER_RADIUS_MAX - PLAYER_RADIUS_INC) {
				player.radius += PLAYER_RADIUS_INC;
			}
			food_spawn(&foods[i], &player);
		}

		if(foods[i].pos.x < _rgl_data->camera->left || foods[i].pos.x > _rgl_data->camera->right || foods[i].pos.y > _rgl_data->camera->bottom || foods[i].pos.y < _rgl_data->camera->top) {
			food_spawn(&foods[i], &player);
		}
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
