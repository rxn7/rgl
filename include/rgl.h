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
        rglAppInitFunc init_f;
        rglAppUpdateFunc update_f;
        rglAppQuitFunc quit_f;
} rglAppDesc;

#include "rgl_platform.h"

b8 rglStart(rglAppDesc *desc);
void rglQuit(void);
void rglSetVsync(b8 value);
void rglGetWindowSize(i32 *w, i32 *h);
f32 rglGetTime(void);

void _rglUpdateProjection(void);

typedef struct _rglAppData {
        rglAppDesc *desc;
	rglAudioContext *audio_cxt;
	RGL_PLATFORM_CONTEXT_T *plat_cxt;

	b8 running;
	i32 width, height;
	rglMat4 projection_matrix;
} _rglAppData;

extern _rglAppData *_rgl_data; /* Defined in rgl.c */

void _rglAppDataCreate(_rglAppData *data, rglAppDesc *desc);
void _rglAppDataDestroy(_rglAppData *data);

void _rglSetupOpenGL(void);
void _rglMainLoop(void);
void _rglDefaultUpdateFunc(f32 dt);

#endif /* __RGL_H */
