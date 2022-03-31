#include "x11/rgl_x11_context.h"
#include "x11/rgl_x11_input.h" 
#include "rgl.h"

#include <stdlib.h> 
#include <time.h>
#include <unistd.h>

static Atom _wm_delete_msg;

static void _rglX11ProcessEvent();

b8
rglX11ContextCreate(rglX11Context *ctx, const char *title, i32 width, i32 height) { 
	RGL_ASSERT_VALID_PTR(ctx);

	ctx->dpy = XOpenDisplay(0);
	RGL_ASSERT_VALID_PTR(ctx->dpy);

	ctx->root = DefaultRootWindow(ctx->dpy);
	RGL_ASSERT_VALID_PTR(ctx->root);

	i32 visual_attribs[] = {
		GLX_RENDER_TYPE, GLX_RGBA_BIT,
		GLX_DOUBLEBUFFER, true,
		GLX_RED_SIZE, 1,
		GLX_GREEN_SIZE, 1,
		GLX_BLUE_SIZE, 1,
		None
	};

	i32 fbcount;
	GLXFBConfig *fbc = glXChooseFBConfig(ctx->dpy, DefaultScreen(ctx->dpy), visual_attribs, &fbcount);
	RGL_ASSERT_VALID_PTR(fbc);

	XVisualInfo *vi = glXGetVisualFromFBConfig(ctx->dpy, fbc[0]);

	XSetWindowAttributes swa;
	swa.colormap = XCreateColormap(ctx->dpy, ctx->root, vi->visual, AllocNone);
	swa.border_pixel = 0;
	swa.event_mask = ExposureMask | KeyPressMask | ButtonPressMask | KeyReleaseMask | ButtonReleaseMask | FocusChangeMask;

	ctx->win = XCreateWindow(ctx->dpy, ctx->root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
	ctx->focus = true;
	RGL_ASSERT_VALID_PTR(ctx->win);

	XMapRaised(ctx->dpy, ctx->win);
	XStoreName(ctx->dpy, ctx->win, title);

	i32 ctx_attribs[] = {
		GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
		GLX_CONTEXT_MINOR_VERSION_ARB, 3,
		None
	};

	ctx->glx = glXCreateContextAttribsARB(ctx->dpy, fbc[0], NULL, true, ctx_attribs);
	RGL_ASSERT_VALID_PTR(ctx->glx);

	glXMakeCurrent(ctx->dpy, ctx->win, ctx->glx);

	_wm_delete_msg = XInternAtom(ctx->dpy, "WM_DELETE_WINDOW", false);
	XSetWMProtocols(ctx->dpy, ctx->win, &_wm_delete_msg, 1);

	return true;
}

void
rglX11ContextDestroy(rglX11Context *ctx) {
	RGL_ASSERT(ctx, false);

	glXMakeCurrent(ctx->dpy, None, 0);
	glXDestroyContext(ctx->dpy, ctx->glx);

	XDestroyWindow(ctx->dpy, ctx->win);
	XCloseDisplay(ctx->dpy);
}

void
rglX11StartFrame(void) {
	while(XPending(_rgl_plat_ctx->dpy)) {
		XNextEvent(_rgl_plat_ctx->dpy, &_rgl_plat_ctx->event);
		_rglX11ProcessEvent();
	}

	rglX11InputUpdate();
}

void
rglX11EndFrame(void) {
	rglX11InputPostUpdate();
	glXSwapBuffers(_rgl_plat_ctx->dpy, _rgl_plat_ctx->win);
}

f32
rglX11GetTime(void) {
	struct timespec time;
	clock_gettime(CLOCK_MONOTONIC, &time);
	return time.tv_sec + ((f64)time.tv_nsec * 0.000000001);
}

void
rglX11SetVsync(b8 value) {
	glXSwapIntervalEXT(_rgl_plat_ctx->dpy, _rgl_plat_ctx->win, value);
}

static void
_rglX11ProcessEvent() {
	switch(_rgl_plat_ctx->event.type) {
		case ClientMessage:
			if(_rgl_plat_ctx->event.xclient.data.l[0] == _wm_delete_msg) {
				rglQuit();
			}

			break;

		case Expose:
			XGetWindowAttributes(_rgl_plat_ctx->dpy, _rgl_plat_ctx->win, &_rgl_plat_ctx->win_attr);
			_rgl_width = _rgl_plat_ctx->win_attr.width;
			_rgl_height = _rgl_plat_ctx->win_attr.height;
			_rglUpdateProjection();
			break;

		case FocusOut:
			_rgl_plat_ctx->focus = false;
			break;

		case FocusIn:
			_rgl_plat_ctx->focus = true;
			break;

		case KeyPress:
			break;

		case ButtonPress:
			if(_rgl_plat_ctx->event.xbutton.button == Button4) {
				_rgl_scroll_value = 1.0f;
			} else if(_rgl_plat_ctx->event.xbutton.button == Button5) {
				_rgl_scroll_value = -1.0f;
			}

			break;
	}
}
