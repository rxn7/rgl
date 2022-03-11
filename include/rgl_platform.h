#pragma once

#if defined(__linux) || defined(__linux__) || defined(linux)
	#define RGL_PLATFORM_LINUX
	#define RGL_PLATFORM_FUN(fun, ...) rgl_x11_##fun(__VA_ARGS__)
	#define RGL_PLATFORM_CONTEXT_T  rgl_x11_context_t

	#include "x11/rgl_x11_context.h"
	#include "x11/rgl_x11_input.h"
#elif defined(_WIN32) || defined(__WIN32)
	#define RGL_PLATFORM_WINDOWS
	#define RGL_PLATFORM_FUN(fun, ...) rgl_win_##fun(__VA_ARGS__)
	#define RGL_PLATFORM_CONTEXT_T  rgl_win_context_t

	#include "win/rgl_win_context.h"
	#include "win/rgl_win_input.h"
#endif
