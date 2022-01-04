#include "x11/rgl_x11_context.h"
#include "x11/rgl_x11_input.h"
#include "rgl.h"

#include <stdlib.h> 
#include <time.h>
#include <unistd.h>

static Atom _wm_delete_msg;

static void _process_event();

void rgl_x11_context_initialize(rgl_x11_context_t *cxt, const char *title, s32 width, s32 height) {
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

	_wm_delete_msg = XInternAtom(cxt->dpy, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(cxt->dpy, cxt->win, &_wm_delete_msg, 1);
}

void rgl_x11_context_destroy(rgl_x11_context_t *cxt) {
	glXMakeCurrent(cxt->dpy, None, 0);
	glXDestroyContext(cxt->dpy, cxt->glx);

	XDestroyWindow(cxt->dpy, cxt->win);
	XCloseDisplay(cxt->dpy);
}

void rgl_x11_start_frame(void) {
	while(XPending(g_data.plat_cxt.dpy)) {
		XNextEvent(g_data.plat_cxt.dpy, &g_data.plat_cxt.event);
		_process_event();
	}

	rgl_x11_input_update();
}

void rgl_x11_end_frame(void) {
	rgl_x11_input_post_update();
	glXSwapBuffers(g_data.plat_cxt.dpy, g_data.plat_cxt.win);
}

f64 rgl_x11_get_time(void) {
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return time.tv_sec + ((f64)time.tv_nsec * 0.000000001);
}

void rgl_x11_set_vsync(b8 value) {
	glXSwapIntervalEXT(g_data.plat_cxt.dpy, g_data.plat_cxt.win, value);
}

static void _process_event() {
	switch(g_data.plat_cxt.event.type) {
		case ClientMessage:
			if(g_data.plat_cxt.event.xclient.data.l[0] == _wm_delete_msg) {
				rgl_quit();
			}

			break;

		case Expose:
			XGetWindowAttributes(g_data.plat_cxt.dpy, g_data.plat_cxt.win, &g_data.plat_cxt.win_attr);
			g_data.width = g_data.plat_cxt.win_attr.width;
			g_data.height = g_data.plat_cxt.win_attr.height;
			rgl_update_projection();
			break;
	}
}
