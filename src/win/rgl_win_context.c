#include "win/rgl_win_context.h"
#include "win/rgl_win_input.h"
#include "rgl.h"

static LRESULT CALLBACK _wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
static void _setup_pixel_format(HDC hdc);

b8 rgl_win_context_initialize(rgl_win_context_t *cxt, const char *title, s32 width, s32 height) { 
	WNDCLASSEX wc;
	memset(&wc, 0, sizeof(WNDCLASSEX));

	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = NULL;
	wc.cbWndExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = "RglWindowClass";
	wc.lpszMenuName = "";
	wc.lpfnWndProc = _wndproc;

	if(!RegisterClass(&wc)) {
		RGL_LOG_ERROR("Failed to register window class.");
		return false;
	}

	cxt->hwnd = CreateWindow("RglWindowClass", title, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, width, height, NULL, NULL, NULL, NULL);
	if(!cxt->hwnd) {
		RGL_LOG_ERROR("Failed to create window.");
		return false;
	}

	ShowWindow(cxt->hwnd, SW_SHOW);
	UpdateWindow(cxt->hwnd);

	return true;
}

void rgl_win_context_destroy(rgl_win_context_t *cxt) {
	if(cxt->hwnd) {
		DestroyWindow(cxt->hwnd);
	}
}

void rgl_win_start_frame(void) {
	printf("Starting frame");
	MSG msg;
	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void rgl_win_end_frame(void) {
}

f32 rgl_win_get_time(void) {
}

void rgl_win_set_vsync(b8 value) {
}

static LRESULT CALLBACK _wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch(msg) {
		case WM_CREATE: {
			HDC hdc = GetDC(g_data.plat_cxt.hwnd);
			_setup_pixel_format(hdc);
			ReleaseDC(g_data.plat_cxt.hwnd, hdc);

			break;
		}

		case WM_DESTROY:
			PostQuitMessage(0);
			rgl_quit();
			break;

		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

static void _setup_pixel_format(HDC hdc) {
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0,
	};

	s32 pf = ChoosePixelFormat(hdc, &pfd);
	if(!pf) {
		RGL_LOG_ERROR("Failed to choose pixel format");
		rgl_quit();
	}

	if(!SetPixelFormat(hdc, pf, &pfd)) {
		RGL_LOG_ERROR("Failed to set pixel format");
		rgl_quit();
	}
}
