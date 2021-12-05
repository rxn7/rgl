#ifndef RGL_H
#define RGL_H

#include <GLFW/glfw3.h>
#include <stdint.h>
#include <stdbool.h>

typedef int8_t          s8;
typedef int16_t         s16;
typedef int32_t         s32;
typedef int64_t         s64;
typedef uint8_t         u8;
typedef uint16_t        u16;
typedef uint32_t        u32;
typedef uint64_t        u64;
typedef float           f32;
typedef double          f64;
typedef bool            b8;

#include <rgl/rgl_color.h>

typedef void (*rgl_app_update_t)(f32 dt);
typedef void (*rgl_app_exit_t)();

typedef struct rgl_app_desc_t {
        const char *title;
        u16 height;
        u16 width;
        rgl_app_update_t update_f;
        rgl_app_exit_t exit_f;
} rgl_app_desc_t;

b8 rgl_init(rgl_app_desc_t *desc);

typedef struct rgl_app_cxt_t {
        GLFWwindow *window;
        rgl_app_desc_t *app_desc;
} rgl_app_cxt_t;

extern rgl_app_cxt_t rgl_cxt_instance;

#endif /* RGL_H */
