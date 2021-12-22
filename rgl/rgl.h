#pragma once

#include <stdio.h>

#include <rgl/glad.h>
#include <GLFW/glfw3.h>

#include "rgl_common.h"
#include "rgl/rgl_color.h"
#include "rgl/rgl_texture.h"

typedef void (*rgl_app_update_t)(f32 dt);
typedef void (*rgl_app_quit_t)();
typedef void (*rgl_app_init_t)();

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

typedef struct rgl_app_cxt_t {
        GLFWwindow *window;
        rgl_app_desc_t *desc;
} rgl_app_cxt_t;

b8 rgl_init(rgl_app_desc_t *desc);
void rgl_render_texture(rgl_texture_t *txt, b8 stretch);
void rgl_get_window_size(u32 *w, u32 *h);
