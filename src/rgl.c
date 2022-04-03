#include "rgl.h"
#include "rgl_log.h"
#include "rgl_shader.h"

#include <sys/resource.h>
#include <sched.h>

RGL_PLATFORM_CONTEXT_T *_rgl_plat_ctx = NULL;
rglAppDesc *_rgl_app_desc = NULL;
rglAudioContext *_rgl_audio_ctx = NULL;
rglCamera *_rgl_camera = NULL;

b8 _rgl_running = false;
i32 _rgl_width, _rgl_height;
f32 _rgl_vp_width, _rgl_vp_height;
f32 _rgl_vp_x, _rgl_vp_y;
f32 _rgl_scroll_value = 0.0f;
u16 _rgl_aspect_x = 0.0f;
u16 _rgl_aspect_y = 0.0f;

void
rglStart(rglAppDesc *desc) {
	RGL_ASSERT_VALID_PTR(desc);

	srand(time(0));

	/* Sanity checks */
        if(desc->width <= 0)            desc->width = 960; 
        if(desc->height <= 0)           desc->height = 640;
        if(!desc->title)                desc->title = "RGL";
        if(!desc->update_f)             desc->update_f = _rglDefaultUpdateFunc;
	if(!desc->draw_f)		desc->draw_f = _rglDefaultDrawFunc;

	/* Calculate the aspect ratio */
	i32 gcd = rglMathGcd_i32(desc->width, desc->height);
	_rgl_aspect_x = desc->width / gcd;
	_rgl_aspect_y = desc->height / gcd;

	_rglInitGlobals(desc);
	_rglSetupOpenGL();
	_rglUpdateProjection();
	_rglShaderCreateDefaults();

	/* Call user-defined init func */
        if(desc->init_f) {
                desc->init_f();
        }
	
        _rglMainLoop();

        rglQuit();
}

void 
rglQuit(void) {
	_rgl_running = false;

	/* Call user-defined quit func */
        if(_rgl_app_desc->quit_f) {
                _rgl_app_desc->quit_f();
        }

	_rglShaderDestroyDefaults();
	_rglDestroyGlobals();

	exit(0);
}

void 
rglSetVsync(b8 value) {
	RGL_PLATFORM_FUN(SetVsync, value);
}

void 
_rglUpdateProjection(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	_rgl_vp_x = 0;
	_rgl_vp_y = 0;
	_rgl_vp_width = _rgl_width;
	_rgl_vp_height = _rgl_height;

	if(_rgl_width * _rgl_aspect_y > _rgl_height * _rgl_aspect_x) {
		_rgl_vp_width = _rgl_height * _rgl_aspect_x / _rgl_aspect_y ;
		_rgl_vp_x = (_rgl_width - _rgl_vp_width) / 2;
	} else if(_rgl_width * _rgl_aspect_y < _rgl_height * _rgl_aspect_x) {
		_rgl_vp_height = _rgl_width * _rgl_aspect_y / _rgl_aspect_x;
		_rgl_vp_y = (_rgl_height - _rgl_vp_height) / 2;
	}

	glViewport(_rgl_vp_x, _rgl_vp_y, _rgl_vp_width, _rgl_vp_height);
	glScissor(_rgl_vp_x, _rgl_vp_y, _rgl_vp_width, _rgl_vp_height);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

f32 
rglGetTime(void) {
	return RGL_PLATFORM_FUN(GetTime);
}

void
_rglInitGlobals(rglAppDesc *desc) {
	RGL_ASSERT_VALID_PTR(desc);

        _rgl_app_desc = desc;

	_rgl_width = desc->width;
	_rgl_height = desc->width;
	_rgl_scroll_value = 0.0f;

	_rgl_plat_ctx = malloc(sizeof(RGL_PLATFORM_CONTEXT_T));
	RGL_ASSERT(RGL_PLATFORM_FUN(ContextCreate, _rgl_plat_ctx, desc->title, desc->width, desc->height), "failed to initialize platform context");

	_rgl_audio_ctx = malloc(sizeof(rglAudioContext));
	rglAudioContextCreate(_rgl_audio_ctx);

	_rgl_camera = malloc(sizeof(rglCamera));
	rglCameraCreate(_rgl_camera, (rglV2){0,0}, 1);
	rglCameraUpdate(_rgl_camera);
}

void
_rglDestroyGlobals() {
	RGL_PLATFORM_FUN(ContextDestroy, _rgl_plat_ctx);
	free(_rgl_plat_ctx);

	rglAudioContextDestroy(_rgl_audio_ctx);
	free(_rgl_audio_ctx);

	rglCameraDestroy(_rgl_camera);
	free(_rgl_camera);
}

void
_rglSetupOpenGL(void) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
_rglMainLoop(void) {
	_rgl_running = true;

	b8 first_frame = true;
        f32 dt = 0, now = RGL_PLATFORM_FUN(GetTime), last = now;
        while(_rgl_running) {
                /* Calculate delta time between frames */
                now = RGL_PLATFORM_FUN(GetTime);
		dt = (f32)(now - last);
                last = now;

		RGL_PLATFORM_FUN(StartFrame);

		/* Black bars viewport */
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glClearColor((f32)_rgl_app_desc->background_color.r / 255.f, (f32)_rgl_app_desc->background_color.g / 255.f, (f32)_rgl_app_desc->background_color.b / 255.f, 1.f);
		glEnable(GL_SCISSOR_TEST);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_SCISSOR_TEST);

		if(!first_frame) {
			_rgl_app_desc->update_f(dt);
		}

		rglCameraUpdate(_rgl_camera);

		_rgl_app_desc->draw_f();

		RGL_PLATFORM_FUN(EndFrame);

		first_frame = false;
        }
}

void
_rglDefaultUpdateFunc(f32 dt) { 
        return;
}

void 
_rglDefaultDrawFunc(void) {
	return;
}
