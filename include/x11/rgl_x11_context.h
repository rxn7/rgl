#pragma once

#include "../rgl_common.h"

#include <time.h>
#include <X11/X.h>

#include <epoxy/gl.h>
#include <epoxy/glx.h>

typedef struct rglX11Context {
	Display *dpy;
	Window win, root;
	XWindowAttributes win_attr;
	XEvent event;
	GLXContext glx;
	b8 focus;
} rglX11Context;

b8 rglX11ContextCreate(rglX11Context *cxt, const char *title, i32 width, i32 height);
void rglX11ContextDestroy(rglX11Context *cxt);

f32 rglX11GetTime(void);
void rglX11StartFrame(void);
void rglX11EndFrame(void);
void rglX11SetVsync(b8 value);
