#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "rgl_common.h"
#include "rgl_texture.h"
#include "rgl_sprite.h"
#include "rgl_input.h"
#include "rgl_immediate.h"
#include "rgl_audio.h"

#ifdef RGL_DEBUG
	#warning Debug mode for RGL is enabled. Debug mode adds additional checks and logs. Undefine RGL_DEBUG to disable.
#endif

typedef struct rgl_app_desc_t {
        const char *title;
        u16 height;
        u16 width;
	rgl_color_t background_color;
        rgl_app_init_t init_f;
        rgl_app_update_t update_f;
        rgl_app_quit_t quit_f;
} rgl_app_desc_t;

#include "rgl_platform.h"

typedef struct rgl_app_data_t {
        rgl_app_desc_t *desc;
	rgl_audio_context_t *audio_cxt;
	RGL_PLATFORM_CONTEXT_T *plat_cxt;

	b8 running;
	s32 width, height;
	mat4 projection_matrix;
} rgl_app_data_t;

b8 rgl_app_data_create(rgl_app_data_t *data, rgl_app_desc_t *desc);
void rgl_app_data_destroy(rgl_app_data_t *data);

extern rgl_app_data_t *g_rgl_data; /* Defined in rgl.c */

b8 rgl_init(rgl_app_desc_t *desc);
void rgl_quit();
void rgl_set_vsync(b8 value);
void rgl_update_projection();
void rgl_get_window_size(s32 *w, s32 *h);
f32 rgl_get_time();
