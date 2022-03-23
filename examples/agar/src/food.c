#include "player.h"
#include "food.h"

void
food_spawn(food_t *food, player_t *player) {
	/* TODO: Don't allow food spawning inside of player */
	f32 x = RAND_RANGE_F32(_rgl_data->camera->left, _rgl_data->camera->right);
	f32 y = RAND_RANGE_F32(_rgl_data->camera->bottom, _rgl_data->camera->top);

	food->pos.x = x;
	food->pos.y = y;

	food->color = RGL_COLOR_RAND;
}

void
food_draw(food_t *food) {
	rglDrawCircle(food->color, food->pos, FOOD_RADIUS);
}
