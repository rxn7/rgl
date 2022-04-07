#include "agar.h"
#include "food.h"
#include "player.h"

void appInit(void);
void appUpdate(f32 dt);
void appQuit(void);
void appDraw(void);
void spawnFoods(void);

Food foods[FOOD_COUNT];
Player player;
f32 zoom = 1.0f;

rglAudioBuffer popSoundBuffer;
rglAudioSource popSoundSources[POP_SOUND_SOURCE_POOL];

int
main(int argc, const char **argv) {
	rglAppDesc desc = (rglAppDesc){
		.title = "RGL | Agar",
		.width = 1280,
		.height = 720,
		.background_color = RGL_RGB(255,255,255),
		.init_f = appInit,
		.update_f = appUpdate,
		.draw_f = appDraw,
		.quit_f = appQuit,
	};

	rglStart(&desc);
}

void
appInit(void) {
	rglAudioBufferLoadFromVorbis(&popSoundBuffer, "./res/pop.ogg");

	for(u32 i=0; i<POP_SOUND_SOURCE_POOL; ++i) {
		rglAudioSourceCreate(&popSoundSources[i], &popSoundBuffer);
	}

	playerSpawn(&player);
	spawnFoods();
}


void
appQuit(void) {
	rglAudioBufferDestroy(&popSoundBuffer);
}

void
appUpdate(f32 dt) {
	if(rglIsKeyJustPressed(RGL_KEY_R)) {
		playerSpawn(&player);
		spawnFoods();
	}

	for(u32 i=0; i<FOOD_COUNT; ++i) {
		foodUpdate(&foods[i], &player, dt);
	}

	playerUpdate(&player, dt);

	zoom += _rgl_scroll_value * 0.05f;
	rglMathClamp(zoom, 0.5f, 1.5f);

	_rgl_camera->zoom = rglMathLerp(_rgl_camera->zoom, 1.f / player.mass * 100 * zoom, dt * 3); 
}

void
appDraw(void) {
	for(u32 i=0; i<FOOD_COUNT; ++i) {
		foodDraw(&foods[i]);
	}

	playerDraw(&player);
}

void
spawnFoods(void) {
	for(u32 i=0; i<FOOD_COUNT; ++i) {
		foodSpawn(&foods[i], &player);
	}
}

void
playPopSound(void) {
	for(u32 i=0; i<POP_SOUND_SOURCE_POOL; ++i) {
		if(!rglAudioSourceIsPlaying(&popSoundSources[i])) {
			rglAudioSourceSetPitch(&popSoundSources[i], RGL_RAND_RANGE_F32(0.9f, 1.1f));
			rglAudioSourcePlay(&popSoundSources[i]);
			return;
		}
	}

	printf("There aren't any ready sound sources!\n");
	rglAudioSourcePlay(&popSoundSources[0]);
}
