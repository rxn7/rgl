#include "rgl_platform.h"
#include "rgl_common.h"
#include "rgl_input.h"
#include "rgl.h"

f32 _rgl_scroll_value = 0.0f;

void
rglGetCursorPos(rglV2 *vec) {
	RGL_PLATFORM_FUN(GetCursorPos, vec);

	vec->x -= _rgl_vp_x;
	vec->y -= _rgl_vp_y;
}

void
rglGetCursorPosInWorld(rglV2 *vec) {
	rglGetCursorPos(vec);

	f32 cam_w = _rgl_camera->right - _rgl_camera->left;
	f32 cam_h = _rgl_camera->bottom - _rgl_camera->top;

	vec->x = _rgl_camera->left + (vec->x / _rgl_vp_width * cam_w);
	vec->y = _rgl_camera->top + (vec->y / _rgl_vp_height * cam_h);
}

b8
rglIsKeyPressed(rglKey key) {
	return RGL_PLATFORM_FUN(IsKeyPressed, key);
}

b8
rglIsKeyJustPressed(rglKey key) {
	return RGL_PLATFORM_FUN(IsKeyJustPressed, key);
}

b8
rglIsKeyJustReleased(rglKey btn) {
	return RGL_PLATFORM_FUN(IsKeyJustReleased, btn);
}

b8
rglIsButtonPressed(rglBtn btn) {
	return RGL_PLATFORM_FUN(IsBtnPressed, btn);
}

b8
rglIsButtonJustPressed(rglBtn btn) {
	return RGL_PLATFORM_FUN(IsBtnJustPressed, btn);
}

b8
rglIsButtonJustReleased(rglBtn btn) {
	return RGL_PLATFORM_FUN(IsBtnJustReleased, btn);
}
