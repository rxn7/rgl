#pragma once

#include "../rgl_common.h"
#include "../glad.h"

#include <X11/X.h>
#include <GL/glx.h>

typedef struct rgl_x11_context_t {
	Display *dpy;
	Window win, root;
	XVisualInfo *visual_info;
	XWindowAttributes win_attr;
	XSetWindowAttributes set_win_attr;
	XEvent event;
	GLXContext glx;
} rgl_x11_context_t;

rgl_x11_context_t *rgl_x11_context_new(const char *title, s32 width, s32 height);
void rgl_x11_context_free(rgl_x11_context_t *cxt);

f32 rgl_x11_get_time(void);
void rgl_x11_start_frame(void);
void rgl_x11_end_frame(void);
void rgl_x11_get_window_size(s32 *width, s32 *height);
