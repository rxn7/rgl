#pragma once

#include "rgl_common.h"
#include "rgl_color.h"

typedef struct rgl_texture_t {
	id_t id;
        u32 width;
        u32 height;
        rgl_color_t *pixels;
} rgl_texture_t;

inline void rgl_texture_fill_pixel_xy(rgl_texture_t *txt, u32 x, u32 y, rgl_color_t color) {
	txt->pixels[y * txt->width + x] = color;
}

inline void rgl_texture_fill_pixel_idx(rgl_texture_t *txt, u32 idx, rgl_color_t color) {
	txt->pixels[idx] = color;
}

inline void rgl_texture_clear(rgl_texture_t *txt) {
        memset(txt->pixels->rgb, 0, txt->width * txt->height * sizeof(rgl_color_t));
}

void rgl_texture_update(rgl_texture_t *txt);
void rgl_texture_initialize(rgl_texture_t *txt, u32 w, u32 h);
void rgl_texture_destroy(rgl_texture_t *txt);
