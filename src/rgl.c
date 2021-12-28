#include "rgl.h"
#include "rgl_log.h"

static b8 _rgl_glfw_init();
static void _rgl_glfw_error_callback(int code, const char *msg);
static void _rgl_start_main_loop();
static void _rgl_quit();
static void _rgl_def_update(f32 dt);

rgl_app_data_t _app_data = {0};
s32 _win_w, _win_h;

b8 rgl_init(rgl_app_desc_t *desc) {
        /* Sanity checks */
        if(desc->width <= 0)            desc->width = 960; 
        if(desc->height <= 0)           desc->height = 640;
        if(!desc->title)                desc->title = "RGL";
        if(!desc->update_f)             desc->update_f = _rgl_def_update;
        if(desc->res_height <= 0)       desc->res_height = 480;
        if(desc->res_width <= 0)        desc->res_width = 640;

        _app_data.desc = desc;

        if(!_rgl_glfw_init()) {
                return false;
        }

        /* Call user defined init func (if it exists) */
        if(desc->init_f) {
                desc->init_f();
        }

        _rgl_start_main_loop();
        _rgl_quit();

        return true;
}

void rgl_render_texture(rgl_texture_t *txt, b8 stretch) {
       if(stretch) {
               glPixelZoom((f32)_win_w / txt->width,(f32)_win_h / txt->height);
       } else {
               glPixelZoom(1, 1);
       }

       glDrawPixels(txt->width, txt->height, GL_RGB, GL_UNSIGNED_BYTE, txt->pixels->rgb);
}

void rgl_get_window_size(u32 *w, u32 *h) {
        *w = _win_w;
        *h = _win_h;
}

static void _rgl_start_main_loop() {
        f32 dt = 0, now = 0, last = 0;
        while(!glfwWindowShouldClose(_app_data.window)) {
                /* Calculate delta time between frames */
                now = glfwGetTime();
                dt = (f32)(now - last);
                last = now;

                glfwPollEvents();
                glfwGetWindowSize(_app_data.window, &_win_w, &_win_h);

                glfwGetCursorPos(_app_data.window, &_app_data.mouse_x, &_app_data.mouse_y);
                _app_data.mouse_y = _win_h - _app_data.mouse_y;

		glClear(GL_COLOR_BUFFER_BIT);

                _app_data.desc->update_f(dt);

                glfwSwapBuffers(_app_data.window);
        }
}

static b8 _rgl_glfw_init() {
	glfwSetErrorCallback(_rgl_glfw_error_callback);
        if(!glfwInit()) {
		RGL_LOG_ERROR("FAILED TO INITIALIZE GLFW");
                return false;
        }

        glfwWindowHint(GLFW_RESIZABLE, true);

        _app_data.window = glfwCreateWindow(_app_data.desc->width, _app_data.desc->height, _app_data.desc->title, NULL, NULL);
        if(!_app_data.window) {
		RGL_LOG_ERROR("FAILED TO CREATE GLFW WINDOW.");
                return false;
        }

	glfwMakeContextCurrent(_app_data.window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        glfwSwapInterval(0);

        return true;
}

static void _rgl_glfw_error_callback(int code, const char *msg) {
	RGL_LOG_ERROR("GLFW ERROR [%i]: %s", code, msg);
}

static void _rgl_quit() {
        /* Call user defined quit function (if it exists) */
        if(_app_data.desc->quit_f) {
                _app_data.desc->quit_f();
        }

        glfwDestroyWindow(_app_data.window);
}

static void _rgl_def_update(f32 dt) { 
        return;
}
