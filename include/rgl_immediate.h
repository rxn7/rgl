#pragma once

#include "rgl_common.h"

#define CIRCLE_MIN_SEGMENTS 12
#define CIRCLE_MAX_SEGMENTS 1000

void rgl_immediate_draw_rect(rglColor col, v2 pos, v2 size);
void rgl_immediate_draw_circle(rglColor col, v2 pos, f32 radius);
void rgl_immediate_draw_line(rglColor col, v2 a, v2 b, f32 width);
void rgl_immediate_draw_circle_outline(rglColor col, v2 pos, f32 radius, f32 line_width);
