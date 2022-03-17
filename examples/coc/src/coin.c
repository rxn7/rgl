#include "common.h"
#include "coin.h"

static rglSprite _sprite = {0};
static rglTexture _texture = {0};
static rglAudioBuffer _pickup_audio_buffer = {0};
static rglAudioSource _pickup_audio_source = {0};

static void coin_render(coin_t *coin);

void coin_initialize() {
	rglTextureLoadFromFile(&_texture, COIN_TEXTURE_PATH, RGL_TEXTURE_FILTER_NONE);
	rglSpriteCreate(&_sprite, &_texture);
	rglV2Setf(&_sprite.size, COIN_SPRITE_SIZE);

	rglAudioBufferLoadFromVorbis(&_pickup_audio_buffer, "res/pickup.ogg");
	rglAudioSourceCreate(&_pickup_audio_source, &_pickup_audio_buffer);
}

void coin_update(coin_t *coin) {
	static const f32 hw = COIN_SPRITE_SIZE / 2.f;
	static const f32 hh = COIN_SPRITE_SIZE / 2.f;

	i32 ww, wh;
	rglGetWindowSize(&ww, &wh);

	if(coin->pos.x < hw) coin->pos.x = hw;
	else if(coin->pos.x > ww - hw) coin->pos.x = ww - hw;

	if(coin->pos.y < hh) coin->pos.y = hh;
	else if(coin->pos.y > wh - hh) coin->pos.y = wh - hh;

	coin_render(coin);
}

static void coin_render(coin_t *coin) {
	rglV2Copy(&coin->pos, &_sprite.position);
	rglSpriteRender(&_sprite);
}

void coin_respawn(coin_t *coin) {
	coin->pos.x = rand() % _rgl_data->width;
	coin->pos.y = rand() % _rgl_data->width;
}

void coin_play_pickup_sound() {
	rglAudioSourceSetPitch(&_pickup_audio_source, RAND_RANGE_F(0.9f, 1.1f));
	rglAudioSourcePlay(&_pickup_audio_source);
}
