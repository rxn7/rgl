#include "rgl_platform.h"
#include "rgl_common.h"
#include "rgl_input.h"
#include "rgl.h"

void rgl_get_cursor_pos(v2 *vec) {
	RGL_PLATFORM_FUN(get_cursor_pos, vec);
}

b8 rgl_is_key_pressed(rglKey key) {
	return RGL_PLATFORM_FUN(is_key_pressed, key);
}

b8 rgl_is_key_just_pressed(rglKey key) {
	return RGL_PLATFORM_FUN(is_key_just_pressed, key);
}

b8 rgl_is_key_just_released(rglKey btn) {
	return RGL_PLATFORM_FUN(is_key_just_released, btn);
}

b8 rgl_is_button_pressed(rglBtn btn) {
	return RGL_PLATFORM_FUN(is_btn_pressed, btn);
}

b8 rgl_is_button_just_pressed(rglBtn btn) {
	return RGL_PLATFORM_FUN(is_btn_just_pressed, btn);
}

b8 rgl_is_button_just_released(rglBtn btn) {
	return RGL_PLATFORM_FUN(is_btn_just_released, btn);
}
