#include "x11/rgl_x11_context.h"
#include "x11/rgl_x11_input.h"
#include "rgl.h"

#include <X11/Xlib.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

static Atom _wm_delete_msg;

static void _rgl_x11_process_event();

rgl_x11_context_t *rgl_x11_context_new(const char *title, s32 width, s32 height) {
	rgl_x11_context_t *cxt = malloc(sizeof(rgl_x11_context_t));

	cxt->dpy = XOpenDisplay(0);
	cxt->root = DefaultRootWindow(cxt->dpy);
	cxt->visual_info = glXChooseVisual(cxt->dpy, 0, (s32[]){GLX_RGBA,GLX_RED_SIZE,1,GLX_GREEN_SIZE,1,GLX_BLUE_SIZE,1,GLX_DOUBLEBUFFER, None});
	cxt->set_win_attr.colormap = XCreateColormap(cxt->dpy, cxt->root, cxt->visual_info->visual, AllocNone);
	cxt->set_win_attr.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | KeyReleaseMask | ButtonReleaseMask;
	cxt->win = XCreateWindow(cxt->dpy, cxt->root, 0, 0, width, height, 0, cxt->visual_info->depth, InputOutput, cxt->visual_info->visual, CWColormap | CWEventMask, &cxt->set_win_attr);

	XMapRaised(cxt->dpy, cxt->win);
	XStoreName(cxt->dpy, cxt->win, title);

	cxt->glx = glXCreateContext(cxt->dpy, cxt->visual_info, NULL, true);
	glXMakeCurrent(cxt->dpy, cxt->win, cxt->glx);
	gladLoadGLLoader((GLADloadproc)glXGetProcAddress);

	_wm_delete_msg = XInternAtom(cxt->dpy, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(cxt->dpy, cxt->win, &_wm_delete_msg, 1);

	return cxt;
}

void rgl_x11_context_free(rgl_x11_context_t *cxt) {
	glXMakeCurrent(cxt->dpy, None, 0);
	glXDestroyContext(cxt->dpy, cxt->glx);

	XDestroyWindow(cxt->dpy, cxt->win);
	XCloseDisplay(cxt->dpy);

	free(cxt);
}

void rgl_x11_start_frame(void) {
	while(XPending(g_app_data.plat_cxt->dpy)) {
		XNextEvent(g_app_data.plat_cxt->dpy, &g_app_data.plat_cxt->event);
		_rgl_x11_process_event();
	}

	rgl_x11_input_update();
}

void rgl_x11_end_frame(void) {
	rgl_x11_input_post_update();
	glXSwapBuffers(g_app_data.plat_cxt->dpy, g_app_data.plat_cxt->win);
}

void rgl_x11_get_window_size(s32 *width, s32 *height) {
	//XGetWindowAttributes(_app_data.plat_cxt->dpy, _app_data.plat_cxt->win, &_app_data.plat_cxt->win_attr);
	*width = g_app_data.plat_cxt->win_attr.width;
	*height = g_app_data.plat_cxt->win_attr.height;
}

f32 rgl_x11_get_time(void) {
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);

	return time.tv_sec + (time.tv_nsec * 0.000000001);
}

static void _rgl_x11_process_event() {
	switch(g_app_data.plat_cxt->event.type) {
		case ClientMessage:
			if(g_app_data.plat_cxt->event.xclient.data.l[0] == _wm_delete_msg) {
				rgl_quit();
			}

			break;

		case Expose:
			XGetWindowAttributes(g_app_data.plat_cxt->dpy, g_app_data.plat_cxt->win, &g_app_data.plat_cxt->win_attr);
			break;
	}
}
