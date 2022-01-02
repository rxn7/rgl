#include "rgl_platform.h"
#include "rgl_common.h"
#include "rgl_input.h"
#include "rgl.h"

void rgl_get_cursor_pos(s32 *x, s32 *y) {
	RGL_PLATFORM_FUN(get_cursor_pos, x, y);
	*y = g_app_data.height - *y;
}

bool rgl_is_key_pressed(rgl_key_t key) {
	return RGL_PLATFORM_FUN(is_key_pressed, key);
}

bool rgl_is_key_just_pressed(rgl_key_t key) {
	return RGL_PLATFORM_FUN(is_key_just_pressed, key);
}

bool rgl_is_button_pressed(rgl_btn_t btn) {
	return RGL_PLATFORM_FUN(is_btn_pressed, btn);
}

bool rgl_is_button_just_pressed(rgl_btn_t btn) {
	return RGL_PLATFORM_FUN(is_btn_just_pressed, btn);
}
