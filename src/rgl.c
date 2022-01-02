#include "rgl.h"
#include "rgl_log.h"

rgl_app_data_t g_app_data = {0};

static void _start_main_loop();
static void _def_update(f32 dt);

b8 rgl_init(rgl_app_desc_t *desc) {
        if(desc->width <= 0)            desc->width = 960; 
        if(desc->height <= 0)           desc->height = 640;
        if(!desc->title)                desc->title = "RGL";
        if(!desc->update_f)             desc->update_f = _def_update;
        if(desc->res_height <= 0)       desc->res_height = 480;
        if(desc->res_width <= 0)        desc->res_width = 640;

        g_app_data.desc = desc;
	g_app_data.plat_cxt = RGL_PLATFORM_FUN(context_new, desc->title, desc->width, desc->height);

        if(desc->init_f) {
                desc->init_f();
        }

	g_app_data.running = true;

        _start_main_loop();

        rgl_quit();

        return true;
}

void rgl_render_texture(rgl_texture_t *txt, b8 stretch) {
       if(stretch)      glPixelZoom((f32)g_app_data.width / txt->width, (f32)g_app_data.height / txt->height);
       else             glPixelZoom(1, 1);

       glDrawPixels(txt->width, txt->height, GL_RGB, GL_UNSIGNED_BYTE, txt->pixels->rgb);
}

void rgl_get_window_size(u32 *w, u32 *h) {
        *w = g_app_data.width;
        *h = g_app_data.height;
}

static void _start_main_loop() {
        f32 dt = 0, now = 0, last = 0;
        while(g_app_data.running) {
                /* Calculate delta time between frames */
                now = RGL_PLATFORM_FUN(get_time);
                dt = (f32)(now - last);
                last = now;

		RGL_PLATFORM_FUN(get_window_size, &g_app_data.width, &g_app_data.height);

		RGL_PLATFORM_FUN(start_frame);
		glClear(GL_COLOR_BUFFER_BIT);

                g_app_data.desc->update_f(dt);

		RGL_PLATFORM_FUN(end_frame);
        }
}

void rgl_quit() {
	g_app_data.running = false;

        if(g_app_data.desc->quit_f) {
                g_app_data.desc->quit_f();
        }

	RGL_PLATFORM_FUN(context_free, g_app_data.plat_cxt);

	exit(0);
}

static void _def_update(f32 dt) { 
        return;
}
