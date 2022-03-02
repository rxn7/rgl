#pragma once

#include "rgl_common.h"

#define CIRCLE_MIN_SEGMENTS 12
#define CIRCLE_MAX_SEGMENTS 1000

void rgl_immediate_draw_rect(rgl_color_t col, vec2 pos, vec2 size);
void rgl_immediate_draw_circle(rgl_color_t col, vec2 pos, f32 radius);
void rgl_immediate_draw_line(rgl_color_t col, vec2 a, vec2 b, f32 width);
