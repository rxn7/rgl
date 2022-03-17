#include "rgl.h"
#include "rgl_log.h"
#include "rgl_shader.h"

#include <sys/resource.h>
#include <sched.h>

_rglAppData *_rgl_data = 0;

b8 rglStart(rglAppDesc *desc) {
	RGL_ASSERT_VALID_PTR(desc);

	/* Sanity checks */
        if(desc->width <= 0)            desc->width = 960; 
        if(desc->height <= 0)           desc->height = 640;
        if(!desc->title)                desc->title = "RGL";
        if(!desc->update_f)             desc->update_f = _rglDefaultUpdateFunc;

	srand(time(0));

	_rgl_data = malloc(sizeof(_rglAppData));
	_rglAppDataCreate(_rgl_data, desc);

	_rglSetupOpenGL();
	_rglUpdateProjection();
	_rglShaderCreateDefaults();

	/* Call user-defined init func */
        if(desc->init_f) {
                desc->init_f();
        }

        _rglMainLoop();

        rglQuit();

        return true;
}

void rglQuit(void) {
	_rgl_data->running = false;

	/* Call user-defined quit func */
        if(_rgl_data->desc->quit_f) {
                _rgl_data->desc->quit_f();
        }

	_rglShaderDestroyDefaults();
	_rglAppDataDestroy(_rgl_data);
	free(_rgl_data);

	exit(0);
}

void rglSetVsync(b8 value) {
	RGL_PLATFORM_FUN(SetVsync, value);
}

void _rglUpdateProjection(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, _rgl_data->width, _rgl_data->height);
	glOrtho(0, _rgl_data->width, _rgl_data->height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	rglMat4Ortho(_rgl_data->projection_matrix, 0, _rgl_data->width, _rgl_data->height, 0, -1, 1);
}

f32 rglGetTime(void) {
	return RGL_PLATFORM_FUN(GetTime);
}

void rglGetWindowSize(i32 *w, i32 *h) {
	*w = _rgl_data->width;
	*h = _rgl_data->height;
}

void _rglAppDataCreate(_rglAppData *data, rglAppDesc *desc) {
	RGL_ASSERT_VALID_PTR(data);
	RGL_ASSERT_VALID_PTR(desc);

        data->desc = desc;
	data->width = desc->width;
	data->height = desc->width;

	data->plat_cxt = malloc(sizeof(RGL_PLATFORM_CONTEXT_T));
	RGL_ASSERT(RGL_PLATFORM_FUN(ContextCreate, data->plat_cxt, desc->title, desc->width, desc->height), "failed to initialize platform context");

	data->audio_cxt = malloc(sizeof(rglAudioContext));
	rglAudioContextCreate(data->audio_cxt);
}

void _rglAppDataDestroy(_rglAppData *data) {
	RGL_PLATFORM_FUN(ContextDestroy, data->plat_cxt);
	free(data->plat_cxt);

	rglAudioContextDestroy(data->audio_cxt);
	free(data->audio_cxt);
}

void _rglSetupOpenGL(void) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor((f32)_rgl_data->desc->background_color.r / 255.f, (f32)_rgl_data->desc->background_color.g / 255.f, (f32)_rgl_data->desc->background_color.b / 255.f, 1.f);
}

void _rglMainLoop(void) {
	_rgl_data->running = true;

        f32 dt = 0, now = RGL_PLATFORM_FUN(GetTime), last = now;
        while(_rgl_data->running) {
                /* Calculate delta time between frames */
                now = RGL_PLATFORM_FUN(GetTime);
		dt = (f32)(now - last);
                last = now;

		RGL_PLATFORM_FUN(StartFrame);
		glClear(GL_COLOR_BUFFER_BIT);

		/* Call user-defined update func */
                _rgl_data->desc->update_f(dt);

		RGL_PLATFORM_FUN(EndFrame);
        }
}

void _rglDefaultUpdateFunc(f32 dt) { 
        return;
}
