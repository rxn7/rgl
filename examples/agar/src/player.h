#ifndef __AGAR_PLAYER_H
#define __AGAR_PLAYER_H

#include <rgl/rgl.h>

#define PLAYER_SPAWN_RADIUS 20
#define PLAYER_MOVE_SPEED 50

typedef struct Player {
	rglV2 position;
	rglColor color;
	f32 mass;
} Player;

void playerSpawn(Player *player);
void playerUpdate(Player *player, f32 dt);
void playerDraw(Player *player);

#endif /* __AGAR_PLAYER_H */
