#include "rgl_platform.h"
#include "rgl_common.h"
#include "rgl_input.h"
#include "rgl.h"

void rglGetCursorPos(v2 *vec) {
	RGL_PLATFORM_FUN(GetCursorPos, vec);
}

b8 rglIsKeyPressed(rglKey key) {
	return RGL_PLATFORM_FUN(IsKeyPressed, key);
}

b8 rglIsKeyJustPressed(rglKey key) {
	return RGL_PLATFORM_FUN(IsKeyJustPressed, key);
}

b8 rglIsKeyJustReleased(rglKey btn) {
	return RGL_PLATFORM_FUN(IsKeyJustReleased, btn);
}

b8 rglIsButtonPressed(rglBtn btn) {
	return RGL_PLATFORM_FUN(IsBtnPressed, btn);
}

b8 rglIsButtonJustPressed(rglBtn btn) {
	return RGL_PLATFORM_FUN(IsBtnJustPressed, btn);
}

b8 rglIsButtonJustReleased(rglBtn btn) {
	return RGL_PLATFORM_FUN(IsBtnJustReleased, btn);
}
