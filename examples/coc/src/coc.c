#include "common.h"
#include "player.h"
#include "coin.h"

void appInit(void);
void appQuit(void);
void appUpdate(f32 dt);
void appDraw(void);
void coinPickupCheck(void);
void playPickupSound(void);
void onCoinPickup(Coin *coin);

rglAudioBuffer pickup_audio_buffer;
rglAudioSource pickup_audio_source;

Player player;
rglLinkedList coins_ll;
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
	rglLinkedListCreate(&coins_ll);

	rglAudioBufferLoadFromVorbis(&pickup_audio_buffer, "res/pickup.ogg");
	rglAudioSourceCreate(&pickup_audio_source, &pickup_audio_buffer);

	playerCreate(&player, PLAYER_TEXTURE_PATH);

	coinsInit();

	for(u32 i=0; i<4; ++i) {
		Coin *coin = malloc(sizeof(Coin));
		coinRespawn(coin);
		rglLinkedListAdd(&coins_ll, coin);
	}
}

void
appQuit(void) {
	playerDestroy(&player);
	rglLinkedListClear(&coins_ll, true);
}

void
appUpdate(f32 dt) {
	coinsUpdate(dt);
	playerUpdate(&player, dt);
	coinPickupCheck();

	rglV2Lerp(&_rgl_camera->position, &player.sprite.transform.position, dt * 10, &_rgl_camera->position);

	_rgl_camera->zoom += _rgl_scroll_value * 0.05f;
	rglMathClamp(_rgl_camera->zoom, .1f, 3.f);
}

void 
appDraw(void) {
	rglLinkedListNode *coin_node = coins_ll.head;
	while(coin_node != NULL) {
		coinDraw((Coin*)coin_node->data);
		coin_node = coin_node->next;
	}

	playerDraw(&player);
}

void
coinPickupCheck(void) {
	rglV2 delta_pos;

	rglLinkedListNode *coin_node = coins_ll.head;
	while(coin_node != NULL) {
		Coin *coin = (Coin*)coin_node->data;

		rglV2Sub(&coin->pos, &player.sprite.transform.position, &delta_pos);
		f32 dist = rglV2Length(&delta_pos);

		if(dist <= PLAYER_COIN_PICKUP_DISTANCE) {
			onCoinPickup(coin);
		}

		coin_node = coin_node->next;
	}
}

void
playPickupSound(void) {
	rglAudioSourceSetPitch(&pickup_audio_source, RGL_RAND_RANGE_F32(0.9f, 1.1f));
	rglAudioSourcePlay(&pickup_audio_source);
}

void
onCoinPickup(Coin *coin) {
	if(++score > 1 && score % 4 == 0) {
		Coin *new_coin = malloc(sizeof(Coin));
		coinRespawn(new_coin);
		rglLinkedListAdd(&coins_ll, new_coin);
		printf("Your coin count has increased, current coin count: %u\n", coins_ll.length);
	}

	coinRespawn(coin);
	playPickupSound();

	/* TODO: UI Text display */
	printf("Score: %u\n", score);
}
