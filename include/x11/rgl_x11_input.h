#pragma once

#include "../rgl_input.h"
#include "rgl_x11_context.h"

void rgl_x11_input_update();
void rgl_x11_input_post_update();

void rgl_x11_get_cursor_pos(s32 *x, s32 *y);

bool rgl_x11_is_key_pressed(rgl_key_t key);
bool rgl_x11_is_key_just_pressed(rgl_key_t key);

bool rgl_x11_is_btn_pressed(rgl_btn_t btn);
bool rgl_x11_is_btn_just_pressed(rgl_btn_t btn);
