#include "block.h"

#define PI_DIGITS 3
#define TIMESTEP 10000

void appUpdate(f32 dt);
void appDraw(void);
void appQuit(void);
void appInit(void);
void onCollide(void);

rglAudioBuffer clack_audio_buffer;
rglAudioSource clack_audio_source;
pieBlock blocks[2];
u64 collisions;

int
main(int argc, const char **argv) {
	rglAppDesc desc = (rglAppDesc) {
		.title = "RGL | Pie",
		.width = 640,
		.height = 480,
		.update_f = appUpdate,
		.draw_f = appDraw,
		.quit_f = appQuit,
		.init_f = appInit,
		.background_color = RGL_WHITE,
	};

	rglStart(&desc);

	return 0;
}

void 
appInit(void) {
	rglAudioBufferLoadFromVorbis(&clack_audio_buffer, "res/clack.ogg");
	rglAudioSourceCreate(&clack_audio_source, &clack_audio_buffer);

	pieBlockCreate(&blocks[0], -300, 30,  -100.f / TIMESTEP, 1);
	pieBlockCreate(&blocks[1],  -150, 80, -100.f / TIMESTEP, powf(100.0f, PI_DIGITS-1));
}

void 
appQuit(void) {
}

void
appUpdate(f32 dt) {
	for(u64 i=0; i<TIMESTEP; ++i) { 
		if(pieBlocksCollide(&blocks[0], &blocks[1])) {
			onCollide();
			pieBlockBounce(&blocks[0], &blocks[1]);
			pieBlockBounce(&blocks[1], &blocks[0]);
		}

		for(u8 b=0; b<2; ++b) {
			if(pieBlockWallCollisionCheck(&blocks[b])) {
				onCollide();
			}

			pieBlockUpdate(&blocks[b], dt);
		}
	}
}

void
appDraw(void) {
	for(u8 i=0; i<2; ++i) {
		pieBlockDraw(&blocks[i]);
	}
}

void onCollide(void) {
	printf("%llu\n", ++collisions);
	rglAudioSourcePlay(&clack_audio_source);
}
