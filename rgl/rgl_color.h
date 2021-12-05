#ifndef RGL_COLOR_H
#define RGL_COLOR_H

typedef struct rgl_color_t {
        union {
                u8 rgba[4]; 
                struct {
                        u8 r, g, b, a;
                };
        };
} rgl_color_t;

#define RGL_RGB(r, g, b) (rgl_color_t) { r, g, b, 255 };
#define RGL_RGBA(r, g, b, a) (rgl_color_t) { r, g, b, a };

#define RGL_COLOR_BLACK         RGL_RGB(0, 0, 0)
#define RGL_COLOR_WHITE         RGL_RGB(255, 255, 255)
#define RGL_COLOR_RED           RGL_RGB(255, 0, 0)
#define RGL_COLOR_GREEN         RGL_RGB(0, 255, 0)
#define RGL_COLOR_BLUE          RGL_RGB(0, 0, 255)

#endif /* RGL_COLOR_H */
