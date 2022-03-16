#include "x11/rgl_x11_input.h"
#include "rgl.h"

static v2 _cursor_pos;

static char _keys[32] = {0};
static char _prev_keys[32] = {0};

static u32 _buttons; 
static u32 _prev_buttons; 

static KeySym _rgl_key_to_keysym(rgl_key_t key) {
	switch(key) {
		case RGL_KEY_A:          return XK_A;
		case RGL_KEY_B:          return XK_B;
		case RGL_KEY_C:          return XK_C;
		case RGL_KEY_D:          return XK_D;
		case RGL_KEY_E:          return XK_E;
		case RGL_KEY_F:          return XK_F;
		case RGL_KEY_G:          return XK_G;
		case RGL_KEY_H:          return XK_H;
		case RGL_KEY_I:          return XK_I;
		case RGL_KEY_J:          return XK_J;
		case RGL_KEY_K:          return XK_K;
		case RGL_KEY_L:          return XK_L;
		case RGL_KEY_M:          return XK_M;
		case RGL_KEY_N:          return XK_N;
		case RGL_KEY_O:          return XK_O;
		case RGL_KEY_P:          return XK_P;
		case RGL_KEY_Q:          return XK_Q;
		case RGL_KEY_R:          return XK_R;
		case RGL_KEY_S:          return XK_S;
		case RGL_KEY_T:          return XK_T;
		case RGL_KEY_U:          return XK_U;
		case RGL_KEY_V:          return XK_V; 
                case RGL_KEY_W:          return XK_W; 
                case RGL_KEY_Y:          return XK_Y;
		case RGL_KEY_Z:          return XK_Z;
		case RGL_KEY_1:		 return XK_1;
		case RGL_KEY_2:		 return XK_2;
		case RGL_KEY_3:		 return XK_3;
		case RGL_KEY_4:		 return XK_4;
		case RGL_KEY_5:		 return XK_5;
		case RGL_KEY_6:		 return XK_6;
		case RGL_KEY_7:		 return XK_7;
		case RGL_KEY_8:		 return XK_8;
		case RGL_KEY_9:		 return XK_9;
		case RGL_KEY_0:		 return XK_0;
		case RGL_KEY_ESC:	 return XK_Escape;
                case RGL_KEY_SPACE:      return XK_space;
                case RGL_KEY_ARROW_DOWN: return XK_Down;
                case RGL_KEY_ARROW_UP:   return XK_Up;
                case RGL_KEY_ARROW_LEFT: return XK_Left;
                case RGL_KEY_ARROW_RIGHT:return XK_Right;
		
		default: break;
	}

	return 0;
}

void rgl_x11_input_update(void) {
	i32 rx, ry;
	i32 mx, my;
	Window win;
	XQueryPointer(_rgl_data->plat_cxt->dpy, _rgl_data->plat_cxt->win, &win, &win, &rx, &ry, &mx, &my, &_buttons);

	_cursor_pos.x = mx;
	_cursor_pos.y = my;

	XQueryKeymap(_rgl_data->plat_cxt->dpy, _keys);
}

void rgl_x11_input_post_update(void) {
	for(u8 i=0; i<32; ++i) {
		_prev_keys[i] = _keys[i];
	}

	_prev_buttons = _buttons;
}

void rgl_x11_get_cursor_pos(v2 *vec) {
	rgl_v2_cpy(&_cursor_pos, vec);
}

b8 rgl_x11_is_key_pressed(rgl_key_t key) {
	KeyCode keycode = XKeysymToKeycode(_rgl_data->plat_cxt->dpy, _rgl_key_to_keysym(key));
	return (_keys[keycode / 8] & (1 << (keycode % 8))) != 0;
}

b8 rgl_x11_is_key_just_pressed(rgl_key_t key) {
	KeyCode keycode = XKeysymToKeycode(_rgl_data->plat_cxt->dpy, _rgl_key_to_keysym(key));
	return (_keys[keycode / 8] & (1 << (keycode % 8))) != 0 && (_prev_keys[keycode / 8] & (1 << (keycode % 8))) == 0;
}

b8 rgl_x11_is_key_just_released(rgl_key_t key) {
	KeyCode keycode = XKeysymToKeycode(_rgl_data->plat_cxt->dpy, _rgl_key_to_keysym(key));
	return (_keys[keycode / 8] & (1 << (keycode % 8))) == 0 && (_prev_keys[keycode / 8] & (1 << (keycode % 8))) != 0;
}

b8 rgl_x11_is_btn_pressed(rgl_btn_t btn) {
	switch(btn) {
		case RGL_MOUSE_LEFT: return _buttons & Button1Mask;
		case RGL_MOUSE_RIGHT: return _buttons & Button3Mask;
		case RGL_MOUSE_MIDDLE: return _buttons & Button2Mask;
		default: break;
	}

	return false;
}

b8 rgl_x11_is_btn_just_pressed(rgl_btn_t btn) {
	switch(btn) {
		case RGL_MOUSE_LEFT: return _buttons & Button1Mask && !(_prev_buttons & Button1Mask);
		case RGL_MOUSE_RIGHT: return _buttons & Button3Mask && !(_prev_buttons & Button3Mask);
		case RGL_MOUSE_MIDDLE: return _buttons & Button2Mask && !(_prev_buttons & Button2Mask);
		default: break;
	}

	return false;
}

b8 rgl_x11_is_btn_just_released(rgl_btn_t btn) {
	switch(btn) {
		case RGL_MOUSE_LEFT: return !(_buttons & Button1Mask) && _prev_buttons & Button1Mask;
		case RGL_MOUSE_RIGHT: return !(_buttons & Button3Mask) && _prev_buttons & Button3Mask;
		case RGL_MOUSE_MIDDLE: return !(_buttons & Button2Mask) && _prev_buttons & Button2Mask;
		default: break;
	}

	return false;
}
