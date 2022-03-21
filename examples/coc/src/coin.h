#pragma once
#include "common.h"

typedef struct coin_t {
	rglV2 pos;
} coin_t;

void coin_initialize();
void coin_destroy(coin_t *coin);
void coin_draw(coin_t *coin);
void coin_respawn(coin_t *coin);
void coin_play_pickup_sound();
