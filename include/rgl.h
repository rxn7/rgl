#ifndef __RGL_H
#define __RGL_H

#include <stdio.h>
#include <stdlib.h>

#include "rgl_common.h"
#include "rgl_texture.h"
#include "rgl_sprite.h"
#include "rgl_input.h"
#include "rgl_immediate.h"
#include "rgl_audio.h"

typedef struct rglAppDesc {
        const char *title;
        u16 height;
        u16 width;
	rglColor background_color;
        rgl_app_init_t init_f;
        rgl_app_update_t update_f;
        rgl_app_quit_t quit_f;
} rglAppDesc;

#include "rgl_platform.h"

b8 rgl_init(rglAppDesc *desc);
void rgl_quit(void);
void rgl_set_vsync(b8 value);
void rgl_get_window_size(i32 *w, i32 *h);
f32 rgl_get_time(void);

void _rgl_update_projection(void);

typedef struct _rgl_app_data_t {
        rglAppDesc *desc;
	rglAudioContext *audio_cxt;
	RGL_PLATFORM_CONTEXT_T *plat_cxt;

	b8 running;
	i32 width, height;
	mat4 projection_matrix;
} _rgl_app_data_t;

extern _rgl_app_data_t *_rgl_data; /* Defined in rgl.c */

void _rgl_app_data_create(_rgl_app_data_t *data, rglAppDesc *desc);
void _rgl_app_data_destroy(_rgl_app_data_t *data);

void _rgl_setup_opengl(void);
void _rgl_main_loop(void);
void _rgl_def_update(f32 dt);

#endif /* __RGL_H */
