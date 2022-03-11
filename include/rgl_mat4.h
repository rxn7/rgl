#pragma once

#include "rgl_common.h"
#include "rgl_v2.h"

typedef struct v2 v2;

typedef f32 mat4[16];

void rgl_mat4_cpy(mat4 src, mat4 dest);
void rgl_mat4_identity(mat4 mat);
void rgl_mat4_ortho(mat4 mat, f32 left, f32 right, f32 bottom, f32 top, f32 z_near, f32 z_far);

void rgl_mat4_mul(mat4 a, mat4 b);
void rgl_mat4_translate(mat4 mat, v2 vec);
void rgl_mat4_scale(mat4 mat, v2 vec);
void rgl_mat4_rotate(mat4 mat, f32 rot);
