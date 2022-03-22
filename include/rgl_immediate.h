#pragma once

#include "rgl_common.h"

#define CIRCLE_MIN_SEGMENTS 12
#define CIRCLE_MAX_SEGMENTS 1000

void rglDrawRect(rglColor col, rglV2 pos, rglV2 size);
void rglDrawRectOutline(rglColor col, rglV2 pos, rglV2 size, f32 line_width);
void rglDrawCircleOutline(rglColor col, rglV2 pos, f32 radius, f32 line_width);
void rglDrawCircle(rglColor col, rglV2 pos, f32 radius);
void rglDrawLine(rglColor col, rglV2 a, rglV2 b, f32 width);
