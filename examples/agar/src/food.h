#ifndef __AGAR_FOOD_H
#define __AGAR_FOOD_H

#include <rgl/rgl.h>
#include "player.h"

typedef struct Food {
	rglV2 position;
	rglColor color;
	f32 mass;
	b8 is_eaten;
} Food;

void foodSpawn(Food *food, Player *player);
void foodDraw(Food *food);
void foodUpdate(Food *food, Player *player, f32 dt);
void foodEat(Food *food);

#endif /* __AGAR_FOOD_H */
