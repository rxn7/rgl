#pragma once

#include "rgl_common.h"

typedef enum rgl_key_t {
	RGL_KEY_UNKNOWN = -1,
	RGL_KEY_A = 0,
	RGL_KEY_B,
	RGL_KEY_C,
	RGL_KEY_D,
	RGL_KEY_E,
	RGL_KEY_F,
	RGL_KEY_G,
	RGL_KEY_H,
	RGL_KEY_I,
	RGL_KEY_J,
	RGL_KEY_K,
	RGL_KEY_L,
	RGL_KEY_M,
	RGL_KEY_N,
	RGL_KEY_O,
	RGL_KEY_P,
	RGL_KEY_Q,
	RGL_KEY_R,
	RGL_KEY_S,
	RGL_KEY_T,
	RGL_KEY_U,
	RGL_KEY_V,
	RGL_KEY_W,
	RGL_KEY_Y,
	RGL_KEY_Z,
	RGL_KEY_1,
	RGL_KEY_2,
	RGL_KEY_3,
	RGL_KEY_4,
	RGL_KEY_5,
	RGL_KEY_6,
	RGL_KEY_7,
	RGL_KEY_8,
	RGL_KEY_9,
	RGL_KEY_0,
	RGL_KEY_ESC,
        RGL_KEY_SPACE,
        RGL_KEY_ARROW_UP,
        RGL_KEY_ARROW_DOWN,
        RGL_KEY_ARROW_LEFT,
        RGL_KEY_ARROW_RIGHT,
        RGL_KEY_COUNT,
} rgl_key_t;

typedef enum rgl_btn_t {
	RGL_MOUSE_UNKNOWN = -1,
        RGL_MOUSE_LEFT = 0,
        RGL_MOUSE_RIGHT,
        RGL_MOUSE_MIDDLE,
        RGL_BTN_COUNT,
} rgl_btn_t;

void rgl_get_cursor_pos(v2 *vec);
bool rgl_is_key_pressed(rgl_key_t key);
bool rgl_is_key_just_pressed(rgl_key_t key);
bool rgl_is_key_just_released(rgl_key_t btn);
bool rgl_is_button_pressed(rgl_btn_t btn);
bool rgl_is_button_just_pressed(rgl_btn_t btn);
bool rgl_is_button_just_released(rgl_btn_t btn);
