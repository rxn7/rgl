#pragma once

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <cglm/cglm.h>

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

#define PI_F 3.1415926f
#define PI_2F 2 * PI_F

#include "rgl_color.h"

typedef void (*rgl_app_update_t)(f64 dt);
typedef void (*rgl_app_quit_t)();
typedef void (*rgl_app_init_t)();
