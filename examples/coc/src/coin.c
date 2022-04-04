#include "common.h"
#include "coin.h"

static rglAnimationTexture _anim_texture = {0};
static rglSpriteAnimator _animator = {0};
static rglSpriteAnimation _animation = {0};
static rglSprite _sprite = {0};
static rglTexture _texture = {0};

void
coinsInit(void) {
	rglTextureLoadFromFile(&_texture, COIN_TEXTURE_PATH, RGL_TEXTURE_FILTER_NONE);

	rglAnimationTextureCreate(&_anim_texture, &_texture, (rglV2){16,16});
	rglSpriteAnimationCreate(&_animation, &_anim_texture, (u16[]){0,1}, 2, 0.2f, true);
	rglSpriteAnimatorCreate(&_animator, &_animation);

	rglSpriteCreate(&_sprite, &_texture);
	rglV2Setf(&_sprite.size, COIN_SPRITE_SIZE);
}

void
coinsUpdate(f32 dt) {
	rglSpriteAnimatorUpdate(&_animator, &_sprite, dt);
}

void
coinDraw(Coin *coin) {
	rglV2Copy(&coin->pos, &_sprite.position);
	rglSpriteRender(&_sprite);
}

void
coinRespawn(Coin *coin) {
	coin->pos.x = RGL_RAND_RANGE_F32(-1000, 1000);
	coin->pos.y = RGL_RAND_RANGE_F32(-1000, 1000);
}
