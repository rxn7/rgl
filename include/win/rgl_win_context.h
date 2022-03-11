#pragma once

#include "../rgl_common.h"

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <epoxy/wgl.h>

typedef struct rgl_win_context_t {
	HWND hwnd;
} rgl_win_context_t;

b8 rgl_win_context_initialize(rgl_win_context_t *cxt, const char *title, s32 width, s32 height);
void rgl_win_context_destroy(rgl_win_context_t *cxt);

f32 rgl_win_get_time(void);
void rgl_win_start_frame(void);
void rgl_win_end_frame(void);
void rgl_win_set_vsync(b8 value);
