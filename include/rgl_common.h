#pragma once

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

typedef f32 v2[2]; 
typedef f32 v3[3];
typedef f32 v4[4];

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

typedef struct rgl_app_data_t {
        GLFWwindow *window;
        rgl_app_desc_t *desc;
        f64 mouse_x, mouse_y;
} rgl_app_data_t;

extern rgl_app_data_t _app_data; /* Defined in rgl.c */
extern s32 _win_w, _win_h; /* Defined in rgl.c */
