#pragma once
#include "common.h"

typedef struct coin_t {
	rgl_sprite_t sprite;
} coin_t;

void coin_create(coin_t *coin);
void coin_destroy(coin_t *coin);
void coin_render(coin_t *coin);
