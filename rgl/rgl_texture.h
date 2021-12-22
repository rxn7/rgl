#pragma once

#include "rgl/rgl_common.h"
#include "rgl/rgl_color.h"

typedef struct rgl_texture_t {
        u32 width;
        u32 height;
        rgl_color_t *pixels;
} rgl_texture_t;

rgl_texture_t *rgl_texture_new(u32 w, u32 h);
void rgl_texture_free(rgl_texture_t *txt);
void rgl_texture_fill_xy(rgl_texture_t *txt, u32 x, u32 y, rgl_color_t color);
void rgl_texture_fill(rgl_texture_t *txt, u32 i, rgl_color_t color);
void rgl_texture_clear(rgl_texture_t *txt);
