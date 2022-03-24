#include "common.h"
#include "coin.h"

static rglAnimationTexture _anim_texture = {0};
static rglSpriteAnimator _animator = {0};
static rglSpriteAnimation _animation = {0};
static rglSprite _sprite = {0};
static rglTexture _texture = {0};
static rglAudioBuffer _pickup_audio_buffer = {0};
static rglAudioSource _pickup_audio_source = {0};

void
coins_initialize() {
	rglTextureLoadFromFile(&_texture, COIN_TEXTURE_PATH, RGL_TEXTURE_FILTER_NONE);

	rglAnimationTextureCreate(&_anim_texture, &_texture, (rglV2){16,16});
	rglSpriteAnimationCreate(&_animation, &_anim_texture, (u16[]){0,1}, 2, 0.2f, true);
	rglSpriteAnimatorCreate(&_animator, &_animation);

	rglSpriteCreate(&_sprite, &_texture);
	rglV2Setf(&_sprite.size, COIN_SPRITE_SIZE);

	rglAudioBufferLoadFromVorbis(&_pickup_audio_buffer, "res/pickup.ogg");
	rglAudioSourceCreate(&_pickup_audio_source, &_pickup_audio_buffer);
}

void
coins_update(f32 dt) {
	rglSpriteAnimatorUpdate(&_animator, &_sprite, dt);
}

void
coin_draw(coin_t *coin) {
	rglV2Copy(&coin->pos, &_sprite.position);
	rglSpriteRender(&_sprite);
}

void
coin_respawn(coin_t *coin) {
	coin->pos.x = RAND_RANGE_F32(-1000, 1000);
	coin->pos.y = RAND_RANGE_F32(-1000, 1000);
}

void
coin_play_pickup_sound() {
	rglAudioSourceSetPitch(&_pickup_audio_source, RAND_RANGE_F32(0.9f, 1.1f));
	rglAudioSourcePlay(&_pickup_audio_source);
}
