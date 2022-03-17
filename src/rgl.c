#include "rgl.h"
#include "rgl_log.h"
#include "rgl_shader.h"

#include <sys/resource.h>
#include <sched.h>

_rgl_app_data_t *_rgl_data = 0;

b8 rgl_init(rglAppDesc *desc) {
	RGL_ASSERT_VALID_PTR(desc);

	/* Sanity checks */
        if(desc->width <= 0)            desc->width = 960; 
        if(desc->height <= 0)           desc->height = 640;
        if(!desc->title)                desc->title = "RGL";
        if(!desc->update_f)             desc->update_f = _rgl_def_update;

	srand(time(0));

	_rgl_data = malloc(sizeof(_rgl_app_data_t));
	_rgl_app_data_create(_rgl_data, desc);

	_rgl_setup_opengl();
	_rgl_update_projection();
	_rgl_shader_create_defaults();

	/* Call user-defined init func */
        if(desc->init_f) {
                desc->init_f();
        }

        _rgl_main_loop();

        rgl_quit();

        return true;
}

void rgl_quit(void) {
	_rgl_data->running = false;

	/* Call user-defined quit func */
        if(_rgl_data->desc->quit_f) {
                _rgl_data->desc->quit_f();
        }

	_rgl_shader_destroy_defaults();

	_rgl_app_data_destroy(_rgl_data);
	free(_rgl_data);

	exit(0);
}

void rgl_set_vsync(b8 value) {
	RGL_PLATFORM_FUN(set_vsync, value);
}

void _rgl_update_projection(void) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, _rgl_data->width, _rgl_data->height);
	glOrtho(0, _rgl_data->width, _rgl_data->height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	rgl_mat4_ortho(_rgl_data->projection_matrix, 0, _rgl_data->width, _rgl_data->height, 0, -1, 1);
}

f32 rgl_get_time(void) {
	return RGL_PLATFORM_FUN(get_time);
}

void rgl_get_window_size(i32 *w, i32 *h) {
	*w = _rgl_data->width;
	*h = _rgl_data->height;
}

void _rgl_app_data_create(_rgl_app_data_t *data, rglAppDesc *desc) {
	RGL_ASSERT_VALID_PTR(data);
	RGL_ASSERT_VALID_PTR(desc);

        data->desc = desc;
	data->width = desc->width;
	data->height = desc->width;

	data->plat_cxt = malloc(sizeof(RGL_PLATFORM_CONTEXT_T));
	RGL_ASSERT(RGL_PLATFORM_FUN(context_initialize, data->plat_cxt, desc->title, desc->width, desc->height), "failed to initialize platform context");

	data->audio_cxt = malloc(sizeof(rglAudioContext));
	rgl_audio_context_create(data->audio_cxt);
}

void _rgl_app_data_destroy(_rgl_app_data_t *data) {
	RGL_PLATFORM_FUN(context_destroy, data->plat_cxt);
	free(data->plat_cxt);

	rgl_audio_context_destroy(data->audio_cxt);
	free(data->audio_cxt);
}

void _rgl_setup_opengl(void) {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor((f32)_rgl_data->desc->background_color.r / 255.f, (f32)_rgl_data->desc->background_color.g / 255.f, (f32)_rgl_data->desc->background_color.b / 255.f, 1.f);
}

void _rgl_main_loop(void) {
	_rgl_data->running = true;

        f32 dt = 0, now = RGL_PLATFORM_FUN(get_time), last = now;
        while(_rgl_data->running) {
                /* Calculate delta time between frames */
                now = RGL_PLATFORM_FUN(get_time);
		dt = (f32)(now - last);
                last = now;

		RGL_PLATFORM_FUN(start_frame);
		glClear(GL_COLOR_BUFFER_BIT);

		/* Call user-defined update func */
                _rgl_data->desc->update_f(dt);

		RGL_PLATFORM_FUN(end_frame);
        }
}

void _rgl_def_update(f32 dt) { 
        return;
}
