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
#include "rgl_camera.h"
#include "rgl_sprite_animator.h"

typedef struct rglAppDesc {
        const char *title;
        u16 height, width;
	rglColor background_color;
        rglAppInitFunc init_f;
        rglAppUpdateFunc update_f;
	rglAppDrawFunc draw_f;
        rglAppQuitFunc quit_f;
} rglAppDesc;

#include "rgl_platform.h"

b8 rglStart(rglAppDesc *desc);
f32 rglGetTime(void);
void rglQuit(void);
void rglSetVsync(b8 value);

extern RGL_PLATFORM_CONTEXT_T *_rgl_plat_ctx;
extern rglAppDesc *_rgl_app_desc;
extern rglAudioContext *_rgl_audio_ctx;
extern rglCamera *_rgl_camera;
extern b8 _rgl_running;
extern i32 _rgl_width, _rgl_height;
extern f32 _rgl_vp_width, _rgl_vp_height;
extern f32 _rgl_vp_x, _rgl_vp_y;
extern f32 _rgl_scroll_value;
extern u16 _rgl_aspect_x;
extern u16 _rgl_aspect_y;

void _rglSetupOpenGL(void);
void _rglMainLoop(void);
void _rglDefaultUpdateFunc(f32 dt);
void _rglDefaultDrawFunc(void);
void _rglUpdateProjection(void);
void _rglInitGlobals(rglAppDesc *desc);
void _rglDestroyGlobals();

#endif /* __RGL_H */
