#include "player.h"
#include "food.h"

void
food_spawn(food_t *food, player_t *player) {
	food->eaten = false;

	/* TODO: Don't allow food spawning inside of player */
	f32 x = RAND_RANGE_F32(_rgl_data->camera->left, _rgl_data->camera->right);
	f32 y = RAND_RANGE_F32(_rgl_data->camera->bottom, _rgl_data->camera->top);

	food->mass = RAND_RANGE_F32(2.f, 4.f);
	food->pos.x = x;
	food->pos.y = y;

	food->color = RGL_COLOR_RAND;
}

void
food_draw(food_t *food) {
	rglDrawCircle(food->color, food->pos, food->mass * 3);
}

void
food_update(food_t *food, player_t *player, f32 dt) {
	rglV2 delta_pos;
	rglV2Sub(&food->pos, &player->pos, &delta_pos);
	f32 dist = rglV2Length(&delta_pos);

	if(dist <= player->radius + food->mass) {
		if(!food->eaten) {
			player->radius += food->mass * 0.1f;
			food_eat(food);
		}
	}

	if(food->pos.x < _rgl_data->camera->left || food->pos.x > _rgl_data->camera->right || food->pos.y > _rgl_data->camera->bottom || food->pos.y < _rgl_data->camera->top) {
		food_spawn(food, player);
	}

	if(dist < (player->radius / 2) + (food->mass * 3 / 2)) {
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
