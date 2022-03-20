#include "player.h"
#include "food.h"

void
food_spawn(food_t *food, player_t *player) {
	/* TODO: Don't allow food spawning inside of player */
	f32 x = RAND_RANGE_F(0, _rgl_data->width);
	f32 y = RAND_RANGE_F(0, _rgl_data->height);

	food->pos.x = x;
	food->pos.y = y;

	food->color = RGL_COLOR_RAND;
}

void
food_render(food_t *food) {
	rglDrawCircle(food->color, food->pos, FOOD_RADIUS);
}
