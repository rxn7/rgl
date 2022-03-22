#include "rgl_platform.h"
#include "rgl_common.h"
#include "rgl_input.h"
#include "rgl.h"

void
rglGetCursorPos(rglV2 *vec) {
	RGL_PLATFORM_FUN(GetCursorPos, vec);
	vec->y = _rgl_data->height - vec->y;
}

/* TODO: Get correct world cursor position. */
void
rglGetCursorPosInWorld(rglV2 *vec) {
	RGL_PLATFORM_FUN(GetCursorPos, vec);

	f32 cam_w = _rgl_data->camera->right - _rgl_data->camera->left;
	f32 cam_h = _rgl_data->camera->bottom - _rgl_data->camera->top;

	vec->x = _rgl_data->camera->left + (vec->x / _rgl_data->width * cam_w);
	vec->y = _rgl_data->camera->top + (vec->y / _rgl_data->height * cam_h);
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
