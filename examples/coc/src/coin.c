#include "common.h"
#include "coin.h"

static rgl_sprite_t sprite = {0};
static rgl_texture_t texture = {0};

void coin_initialize() {
	rgl_texture_load_from_file(&texture, COIN_TEXTURE_PATH, RGL_TEXTURE_FILTER_NONE);
	sprite.texture = &texture;
	rgl_sprite_create(&sprite);
	rgl_v2_setf(&sprite.size, COIN_SPRITE_SIZE);
}

void coin_render(coin_t *coin) {
	rgl_v2_cpy(&coin->pos, &sprite.position);
	rgl_sprite_render(&sprite);
}

void coin_respawn(coin_t *coin) {
	coin->pos.x = rand() % g_rgl_data->width;
	coin->pos.y = rand() % g_rgl_data->width;
}
