#pragma once
#include "common.h"

typedef struct Coin {
	rglV2 pos;
} Coin;

void coinsInit(void);
void coinsUpdate(f32 dt);
void coinDestroy(Coin *coin);
void coinDraw(Coin *coin);
void coinRespawn(Coin *coin);
