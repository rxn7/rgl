#ifndef __AGAR_FOOD_H
#define __AGAR_FOOD_H

#include <rgl/rgl.h>
#include "player.h"

typedef struct food_t {
	rglV2 pos;
	rglColor color;
	f32 mass;
	b8 eaten;
} food_t;

void food_spawn(food_t *food, player_t *player);
void food_draw(food_t *food);
void food_update(food_t *food, player_t *player, f32 dt);
void food_eat(food_t *food);

#endif /* __AGAR_FOOD_H */
