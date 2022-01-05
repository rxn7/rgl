#pragma once

#include "rgl_common.h"

typedef struct rgl_transform_t {
	vec2 position;
	vec2 scale;
	f32 rotation;
} rgl_transform_t;

void rgl_transform_initialize(rgl_transform_t *trans);
void rgl_transform_calculate(mat4 dest, rgl_transform_t *trans);
