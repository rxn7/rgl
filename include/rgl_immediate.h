#pragma once

#include "rgl_common.h"

#define CIRCLE_MIN_SEGMENTS 12
#define CIRCLE_MAX_SEGMENTS 1000

void rglDrawRect(rglColor col, v2 pos, v2 size);
void rglDrawRectOutline(rglColor col, v2 pos, v2 size, f32 line_width);
void rglDrawCircleOutline(rglColor col, v2 pos, f32 radius, f32 line_width);
void rglDrawCircle(rglColor col, v2 pos, f32 radius);
void rglDrawLine(rglColor col, v2 a, v2 b, f32 width);
