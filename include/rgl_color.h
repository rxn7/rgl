#pragma once

#include "rgl_common.h"

typedef struct rgl_color_t {
        union {
                u8 rgb[3]; 
                struct {
                        u8 r, g, b;
                };
        };
} rgl_color_t;

#define RGL_RGB(r, g, b) (rgl_color_t) { .rgb = {r, g, b}  }

#define RAND_255 (rand() % 255) 
#define RGL_COLOR_RAND ((rgl_color_t){.rgb = {RAND_255,RAND_255,RAND_255}})

#define RGL_BLACK         RGL_RGB(0, 0, 0)
#define RGL_WHITE         RGL_RGB(255, 255, 255)
#define RGL_RED           RGL_RGB(255, 0, 0)
#define RGL_GREEN         RGL_RGB(0, 255, 0)
#define RGL_BLUE          RGL_RGB(0, 0, 255)
