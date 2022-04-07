#include "player.h"
#include "food.h"
#include "agar.h"

#define FOOD_RADIUS(f) (food->mass * 100.f)

void
foodSpawn(Food *food, Player *player) {
	food->is_eaten = false;

	f32 x = player->position.x + RGL_RAND_RANGE_F32(100, 1000) * (roundf(RGL_RAND_F32) * 2 - 1);
	f32 y = player->position.y + RGL_RAND_RANGE_F32(100, 1000) * (roundf(RGL_RAND_F32) * 2 - 1);

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

	if(dist > 1000) {
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
	playPopSound();
}
