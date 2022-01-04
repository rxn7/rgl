#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "rgl_common.h"
#include "rgl_color.h"
#include "rgl_texture.h"

typedef struct rgl_app_desc_t {
        const char *title;
        u16 height;
        u16 width;
        rgl_app_init_t init_f;
        rgl_app_update_t update_f;
        rgl_app_quit_t quit_f;
        u32 res_width;
        u32 res_height;
} rgl_app_desc_t;

#include "rgl_platform.h"

typedef struct rgl_app_data_t {
        rgl_app_desc_t *desc;
	RGL_PLATFORM_CONTEXT_T plat_cxt;
	bool running;
	s32 width, height;
} rgl_app_data_t;

extern rgl_app_data_t g_data; /* Defined in rgl.c */

b8 rgl_init(rgl_app_desc_t *desc);
void rgl_quit();
void rgl_set_vsync(b8 value);
void rgl_update_projection();
