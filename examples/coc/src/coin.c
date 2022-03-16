#include "common.h"
#include "coin.h"

static rgl_sprite_t _sprite = {0};
static rgl_texture_t _texture = {0};
static rgl_audio_buffer_t _pickup_audio_buffer = {0};
static rgl_audio_source_t _pickup_audio_source = {0};

static void coin_render(coin_t *coin);

void coin_initialize() {
	rgl_texture_load_from_file(&_texture, COIN_TEXTURE_PATH, RGL_TEXTURE_FILTER_NONE);
	rgl_sprite_create(&_sprite, &_texture);
	rgl_v2_setf(&_sprite.size, COIN_SPRITE_SIZE);

	rgl_audio_buffer_load_from_vorbis(&_pickup_audio_buffer, "res/pickup.ogg");
	rgl_audio_source_create(&_pickup_audio_source, &_pickup_audio_buffer);
}

void coin_update(coin_t *coin) {
	static const f32 hw = COIN_SPRITE_SIZE / 2.f;
	static const f32 hh = COIN_SPRITE_SIZE / 2.f;

	if(coin->pos.x < hw) coin->pos.x = hw;
	else if(coin->pos.x > g_rgl_data->width - hw) coin->pos.x = g_rgl_data->width - hw;

	if(coin->pos.y < hh) coin->pos.y = hh;
	else if(coin->pos.y > g_rgl_data->height - hh) coin->pos.y = g_rgl_data->height - hh;

	coin_render(coin);
}

static void coin_render(coin_t *coin) {
	rgl_v2_cpy(&coin->pos, &sprite.position);
	rgl_sprite_render(&sprite);
}

void coin_respawn(coin_t *coin) {
	coin->pos.x = rand() % g_rgl_data->width;
	coin->pos.y = rand() % g_rgl_data->width;
}

void coin_play_pickup_sound() {
	rgl_audio_source_set_pitch(&_pickup_audio_source, RAND_RANGE_F(0.9f, 1.1f));
	rgl_audio_source_play(&_pickup_audio_source);
}
