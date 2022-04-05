#include <rgl/rgl.h>

void appInit(void);
void appUpdate(f32 dt);
void appQuit(void);
void appDraw(void);

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
}

void
appUpdate(f32 dt) {
	printf("FPS: %f\n", 1.f / dt);
}

void
appDraw(void) {
	rglDrawCircleOutline(RGL_RED, (rglV2){-450,0}, 100, 4);
	rglDrawCircle(RGL_RGB(255,255,0), (rglV2){-200,0}, 100);
	rglDrawRect(RGL_RGB(255,150,0), (rglV2){200,0}, (rglV2){100,100});
	rglDrawRectOutline(RGL_GREEN, (rglV2){450,0}, (rglV2){100,100}, 4);
}

void
appQuit(void) {
	puts("Quitting...");
}
