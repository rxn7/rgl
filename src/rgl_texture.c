#include <stdlib.h>
#include <string.h>

#include "rgl_texture.h"

rgl_texture_t *rgl_texture_new(u32 w, u32 h) {
        rgl_texture_t *txt = malloc(sizeof(rgl_texture_t));
        txt->width = w;
        txt->height = h;
        txt->pixels = calloc(w * h, sizeof(rgl_color_t));

        return txt; 
}

void rgl_texture_free(rgl_texture_t *txt) {
        free(txt->pixels);
        free(txt);
}

void rgl_texture_fill_xy(rgl_texture_t *txt, u32 x, u32 y, rgl_color_t col) {
        txt->pixels[y * txt->width + x] = col;
}

void rgl_texture_fill(rgl_texture_t *txt, u32 i, rgl_color_t col) {
        txt->pixels[i] = col;
}

void rgl_texture_clear(rgl_texture_t *txt) {
        memset(txt->pixels->rgb, 0, txt->width * txt->height * sizeof(rgl_color_t));
}
