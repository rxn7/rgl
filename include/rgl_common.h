#pragma once

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <epoxy/gl.h>

#define PI_F 3.1415926f
#define PI_2F 2.0f*PI_F

#define RAND_F ((f32)rand() / (f32)RAND_MAX)
#define RAND_255 (rand() % 256) 
#define RAND_RANGE_F(min, max) (RAND_F * (max-min) + min)

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

#include "rgl_v2.h"
#include "rgl_color.h"
#include "rgl_mat4.h"
#include "rgl_log.h"

typedef void (*rgl_app_update_t)(f32 dt);
typedef void (*rgl_app_quit_t)();
typedef void (*rgl_app_init_t)();
