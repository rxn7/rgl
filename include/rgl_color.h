#ifndef __RGL_COLOR_H
#define __RGL_COLOR_H

#include "rgl_common.h"

typedef struct rglColor {
        union {
                u8 rgb[3]; 
                struct {
                        u8 r, g, b;
                };
        };
} rglColor;

#define RGL_RGB(r, g, b) (rglColor) { .rgb = {r, g, b}  }
#define RGL_U8_TO_F32(col) (f32)col.r / 255.f, (f32)col.g / 255.f, (f32)col.b / 255.f

#define RGL_COLOR_RAND ((rglColor){.rgb = {RAND_U8,RAND_U8,RAND_U8}})

#define RGL_BLACK         RGL_RGB(0, 0, 0)
#define RGL_WHITE         RGL_RGB(255, 255, 255)
#define RGL_RED           RGL_RGB(255, 0, 0)
#define RGL_GREEN         RGL_RGB(0, 255, 0)
#define RGL_BLUE          RGL_RGB(0, 0, 255)

#endif /* __RGL_COLOR_H */
