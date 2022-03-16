#include "x11/rgl_x11_context.h"
#include "x11/rgl_x11_input.h" 
#include "rgl.h"

#include <stdlib.h> 
#include <time.h>
#include <unistd.h>

static Atom _wm_delete_msg;

static void _process_event();

b8 rgl_x11_context_initialize(rgl_x11_context_t *cxt, const char *title, i32 width, i32 height) { 
	RGL_ASSERT_VALID_PTR(cxt);

	cxt->dpy = XOpenDisplay(0);
	RGL_ASSERT_VALID_PTR(cxt->dpy);

	cxt->root = DefaultRootWindow(cxt->dpy);
	RGL_ASSERT_VALID_PTR(cxt->root);

	i32 visual_attribs[] = {
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_DOUBLEBUFFER, true,
		GLX_RED_SIZE, 1,
		GLX_GREEN_SIZE, 1,
		GLX_BLUE_SIZE, 1,
		None
	};

	i32 fbcount;
	GLXFBConfig *fbc = glXChooseFBConfig(cxt->dpy, DefaultScreen(cxt->dpy), visual_attribs, &fbcount);
	RGL_ASSERT_VALID_PTR(fbc);

	XVisualInfo *vi = glXGetVisualFromFBConfig(cxt->dpy, fbc[0]);

	XSetWindowAttributes swa;
	swa.colormap = XCreateColormap(cxt->dpy, cxt->root, vi->visual, AllocNone);
	swa.border_pixel = 0;
	swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | KeyReleaseMask | ButtonReleaseMask;

	cxt->win = XCreateWindow(cxt->dpy, cxt->root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
	RGL_ASSERT_VALID_PTR(cxt->win);

	XMapRaised(cxt->dpy, cxt->win);
	XStoreName(cxt->dpy, cxt->win, title);

	i32 cxt_attribs[] = {
		GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 3,
		None
	};

	cxt->glx = glXCreateContextAttribsARB(cxt->dpy, fbc[0], NULL, true, cxt_attribs);
	RGL_ASSERT_VALID_PTR(cxt->glx);

	glXMakeCurrent(cxt->dpy, cxt->win, cxt->glx);

	_wm_delete_msg = XInternAtom(cxt->dpy, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(cxt->dpy, cxt->win, &_wm_delete_msg, 1);

	return true;
}

void rgl_x11_context_destroy(rgl_x11_context_t *cxt) {
	RGL_ASSERT(cxt, false);

	glXMakeCurrent(cxt->dpy, None, 0);
	glXDestroyContext(cxt->dpy, cxt->glx);

	XDestroyWindow(cxt->dpy, cxt->win);
	XCloseDisplay(cxt->dpy);
}

void rgl_x11_start_frame(void) {
	while(XPending(_rgl_data->plat_cxt->dpy)) {
		XNextEvent(_rgl_data->plat_cxt->dpy, &_rgl_data->plat_cxt->event);
		_process_event();
	}

	rgl_x11_input_update();
}

void rgl_x11_end_frame(void) {
	rgl_x11_input_post_update();
	glXSwapBuffers(_rgl_data->plat_cxt->dpy, _rgl_data->plat_cxt->win);
}

f32 rgl_x11_get_time(void) {
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return time.tv_sec + ((f64)time.tv_nsec * 0.000000001);
}

void rgl_x11_set_vsync(b8 value) {
	glXSwapIntervalEXT(_rgl_data->plat_cxt->dpy, _rgl_data->plat_cxt->win, value);
}

static void _process_event() {
	switch(_rgl_data->plat_cxt->event.type) {
		case ClientMessage:
			if(_rgl_data->plat_cxt->event.xclient.data.l[0] == _wm_delete_msg) {
				rgl_quit();
			}

			break;

		case Expose:
			XGetWindowAttributes(_rgl_data->plat_cxt->dpy, _rgl_data->plat_cxt->win, &_rgl_data->plat_cxt->win_attr);
			_rgl_data->width = _rgl_data->plat_cxt->win_attr.width;
			_rgl_data->height = _rgl_data->plat_cxt->win_attr.height;
			_rgl_update_projection();
			break;
	}
}
