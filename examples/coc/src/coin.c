#include "common.h"
#include "coin.h"

static rgl_sprite_t sprite = {0};
static rgl_texture_t texture = {0};
static rgl_audio_buffer_t pickup_audio_buffer = {0};
static rgl_audio_source_t pickup_audio_source = {0};

void coin_initialize() {
	rgl_texture_load_from_file(&texture, COIN_TEXTURE_PATH, RGL_TEXTURE_FILTER_NONE);
	sprite.texture = &texture;
	rgl_sprite_create(&sprite);
	rgl_v2_setf(&sprite.size, COIN_SPRITE_SIZE);

	rgl_audio_buffer_create_from_vorbis(&pickup_audio_buffer, "res/pickup.ogg");
	rgl_audio_source_create(&pickup_audio_source, &pickup_audio_buffer);
}

void coin_render(coin_t *coin) {
	rgl_v2_cpy(&coin->pos, &sprite.position);
	rgl_sprite_render(&sprite);
}

void coin_respawn(coin_t *coin) {
	coin->pos.x = rand() % g_rgl_data->width;
	coin->pos.y = rand() % g_rgl_data->width;
}

void coin_play_pickup_sound() {
	rgl_audio_source_set_pitch(&pickup_audio_source, RAND_RANGE_F(0.9f, 1.1f));
	rgl_audio_source_play(&pickup_audio_source);
}
