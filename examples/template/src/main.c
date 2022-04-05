#include <rgl/rgl.h>

void appInit(void);
void appUpdate(f32 dt);
void appQuit(void);
void appDraw(void);
void randomizeColors(void);

rglColor colors[4] = {};

int
main(int argc, const char **argv) {
	rglAppDesc desc = (rglAppDesc){
		.title = "RGL | Template",
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
	puts("RGL has initialised.");
	randomizeColors();
}

void
appUpdate(f32 dt) {
	printf("FPS: %f\n", 1.f / dt);

	if(rglIsKeyJustPressed(RGL_KEY_SPACE)) {
		randomizeColors();
	}
}

void
appDraw(void) {
	rglDrawCircleOutline(colors[0], (rglV2){-450,0}, 100, 4);
	rglDrawCircle(colors[1], (rglV2){-200,0}, 100);
	rglDrawRect(colors[2], (rglV2){200,0}, (rglV2){100,100});
	rglDrawRectOutline(colors[3], (rglV2){450,0}, (rglV2){100,100}, 4);
}

void
appQuit(void) {
	puts("Quitting...");
}

void
randomizeColors(void) {
	for(u8 i=0; i<4; ++i) {
		colors[i] = RGL_COLOR_RAND;
	}
}
