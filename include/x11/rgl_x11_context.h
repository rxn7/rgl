#pragma once

#include "../rgl_common.h"

#include <time.h>
#include <X11/X.h>

#include <epoxy/gl.h>
#include <epoxy/glx.h>

typedef struct rgl_x11_context_t {
	Display *dpy;
	Window win, root;
	XWindowAttributes win_attr;
	XEvent event;
	GLXContext glx;
} rgl_x11_context_t;

b8 rgl_x11_context_initialize(rgl_x11_context_t *cxt, const char *title, i32 width, i32 height);
void rgl_x11_context_destroy(rgl_x11_context_t *cxt);

f32 rgl_x11_get_time(void);
void rgl_x11_start_frame(void);
void rgl_x11_end_frame(void);
void rgl_x11_set_vsync(b8 value);
