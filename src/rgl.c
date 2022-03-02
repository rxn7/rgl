#include "rgl.h"
#include "rgl_log.h"
#include "rgl_shader.h"

rgl_app_data_t g_data = {0};

static void _start_main_loop();
static void _def_update(f64 dt);

b8 rgl_init(rgl_app_desc_t *desc) {
        if(desc->width <= 0)            desc->width = 960; 
        if(desc->height <= 0)           desc->height = 640;
        if(!desc->title)                desc->title = "RGL";
        if(!desc->update_f)             desc->update_f = _def_update;

        g_data.desc = desc;

	RGL_PLATFORM_FUN(context_initialize, &g_data.plat_cxt, desc->title, desc->width, desc->height);
	rgl_update_projection();

        if(desc->init_f) {
                desc->init_f();
        }

	rgl_shader_create_primitives();

        _start_main_loop();

        rgl_quit();

        return true;
}

void rgl_quit() {
	g_data.running = false;

        if(g_data.desc->quit_f) {
                g_data.desc->quit_f();
        }

	RGL_PLATFORM_FUN(context_destroy, &g_data.plat_cxt);

	exit(0);
}

void rgl_set_vsync(b8 value) {
	RGL_PLATFORM_FUN(set_vsync, value);
}

void rgl_update_projection() {
	glViewport(0, 0, g_data.width, g_data.height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
}

f32 rgl_get_time() {
	return RGL_PLATFORM_FUN(get_time);
}

static void _start_main_loop() {
	g_data.running = true;

        f64 dt = 0, now = RGL_PLATFORM_FUN(get_time), last = now;
        while(g_data.running) {
                /* Calculate delta time between frames */
                now = RGL_PLATFORM_FUN(get_time);
		dt = (f32)(now - last);
                last = now;

		RGL_PLATFORM_FUN(start_frame);
		glClear(GL_COLOR_BUFFER_BIT);

                g_data.desc->update_f(dt);

		RGL_PLATFORM_FUN(end_frame);
        }
}

static void _def_update(f64 dt) { 
        return;
}
