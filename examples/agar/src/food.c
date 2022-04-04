#include "player.h"
#include "food.h"

#define FOOD_RADIUS(f) (food->mass * 100.f)

void
foodSpawn(Food *food, Player *player) {
	food->is_eaten = false;

	/* TODO: Don't allow food spawning inside of player */
	f32 x = RGL_RAND_RANGE_F32(_rgl_camera->left, _rgl_camera->right);
	f32 y = RGL_RAND_RANGE_F32(_rgl_camera->bottom, _rgl_camera->top);

	food->mass = RGL_RAND_RANGE_F32(0.01f, 0.02f);
	food->position.x = x;
	food->position.y = y;

	food->color = RGL_COLOR_RAND;
}

void
foodDraw(Food *food) {
	rglDrawCircle(food->color, food->position, FOOD_RADIUS(food));
}

void
foodUpdate(Food *food, Player *player, f32 dt) {
	rglV2 delta_pos;
	rglV2Sub(&food->position, &player->position, &delta_pos);
	f32 dist = rglV2Length(&delta_pos);

	if(dist <= player->mass + FOOD_RADIUS(food->mass)) {
		if(!food->is_eaten) {
			player->mass += food->mass;
			foodEat(food);
		}
	}

	if(food->position.x + FOOD_RADIUS(food) < _rgl_camera->left || food->position.x - FOOD_RADIUS(food) > _rgl_camera->right || food->position.y - FOOD_RADIUS(food) > _rgl_camera->bottom || food->position.y + FOOD_RADIUS(food) < _rgl_camera->top) {
		foodSpawn(food, player);
	}

	if(food->is_eaten) {
		food->mass -= dt * 0.05f;
		rglV2Lerp(&food->position, &player->position, dt * 1.01f, &food->position);

		if(food->mass <= 0) {
			foodSpawn(food, player);
		}
	}
}

void
foodEat(Food *food) {
	food->is_eaten = true;
}
