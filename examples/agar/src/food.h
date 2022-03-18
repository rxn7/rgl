#ifndef __AGAR_FOOD_H
#define __AGAR_FOOD_H

#include <rgl/rgl.h>
#include "player.h"

#define FOOD_RADIUS 5

typedef struct food_t {
	rglV2 pos;
	rglColor color;
} food_t;

void food_spawn(food_t *food, player_t *player);
void food_render(food_t *food);

#endif /* __AGAR_FOOD_H */
