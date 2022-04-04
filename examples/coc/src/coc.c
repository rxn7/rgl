#include "common.h"
#include "player.h"
#include "coin.h"

void appInit(void);
void appQuit(void);
void appUpdate(f32 dt);
void appDraw(void);
void coinPickupCheck(void);
void playPickupSound(void);

rglAudioBuffer pickup_audio_buffer;
rglAudioSource pickup_audio_source;

Player player;
Coin coins[COIN_COUNT];
u32 score = 0;

int 
main(int argc, const char **argv) {
	rglAppDesc desc = (rglAppDesc) {
		.width = 1280,
		.height = 720,
		.title = "RGL | Coc",
		.background_color = RGL_WHITE,
		.update_f = appUpdate,
		.draw_f = appDraw,
		.init_f = appInit,
		.quit_f = appQuit,
	};

	rglStart(&desc);
}

void
appInit(void) {
	rglAudioBufferLoadFromVorbis(&pickup_audio_buffer, "res/pickup.ogg");
	rglAudioSourceCreate(&pickup_audio_source, &pickup_audio_buffer);

	playerCreate(&player, PLAYER_TEXTURE_PATH);
	coinsInit();

	for(u32 i=0; i<COIN_COUNT; ++i) {
		coinRespawn(&coins[i]);
	}
}

void
appQuit(void) {
	playerDestroy(&player);
}

void
appUpdate(f32 dt) {
	coinsUpdate(dt);
	playerUpdate(&player, dt);
	coinPickupCheck();

	rglV2Lerp(&_rgl_camera->position, &player.sprite.position, dt * 10, &_rgl_camera->position);

	_rgl_camera->zoom += _rgl_scroll_value * 0.05f;
	_rgl_camera->zoom = rglMathClamp_f32(_rgl_camera->zoom, .1f, 3.f);
}

void 
appDraw(void) {
	for(u32 i=0; i<COIN_COUNT; ++i) {
		coinDraw(&coins[i]);
	}

	playerDraw(&player);
}

void
coinPickupCheck(void) {
	rglV2 delta_pos;

	for(u32 i=0; i<COIN_COUNT; ++i) {
		rglV2Sub(&coins[i].pos, &player.sprite.position, &delta_pos);
		f32 dist = rglV2Length(&delta_pos);

		if(dist <= PLAYER_COIN_PICKUP_DISTANCE) {
			coinRespawn(&coins[i]);
			playPickupSound();

			/* TODO: UI Text display */
			printf("Score: %u\n", ++score);
		}
	}
}

void
playPickupSound(void) {
	rglAudioSourceSetPitch(&pickup_audio_source, RGL_RAND_RANGE_F32(0.9f, 1.1f));
	rglAudioSourcePlay(&pickup_audio_source);
}
