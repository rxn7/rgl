#include "player.h"
#include "food.h"

#define FOOD_RADIUS(f) (food->mass * 100.f)

void
food_spawn(food_t *food, player_t *player) {
	food->eaten = false;

	/* TODO: Don't allow food spawning inside of player */
	f32 x = RAND_RANGE_F32(_rgl_data->camera->left, _rgl_data->camera->right);
	f32 y = RAND_RANGE_F32(_rgl_data->camera->bottom, _rgl_data->camera->top);

	food->mass = RAND_RANGE_F32(0.01f, 0.02f);
	food->pos.x = x;
	food->pos.y = y;

	food->color = RGL_COLOR_RAND;
}

void
food_draw(food_t *food) {
	rglDrawCircle(food->color, food->pos, FOOD_RADIUS(food));
}

void
food_update(food_t *food, player_t *player, f32 dt) {
	rglV2 delta_pos;
	rglV2Sub(&food->pos, &player->pos, &delta_pos);
	f32 dist = rglV2Length(&delta_pos);

	if(dist <= player->mass + food->mass) {
		if(!food->eaten) {
			player->mass += food->mass;
			food_eat(food);
		}
	}

	if(food->pos.x + FOOD_RADIUS(food) < _rgl_data->camera->left || food->pos.x - FOOD_RADIUS(food) > _rgl_data->camera->right || food->pos.y - FOOD_RADIUS(food) > _rgl_data->camera->bottom || food->pos.y + FOOD_RADIUS(food) < _rgl_data->camera->top) {
		food_spawn(food, player);
	}

	if(dist < (player->mass / 2) + (FOOD_RADIUS(food) / 2)) {
		food_spawn(food, player);
	} else if(food->eaten) {
		food->mass -= dt * 2.f;
		rglV2Lerp(&food->pos, &player->pos, dt * 3.5f, &food->pos);

		if(food->mass <= 0) {
			food_spawn(food, player);
		}
	}
}

void
food_eat(food_t *food) {
	food->eaten = true;
}
