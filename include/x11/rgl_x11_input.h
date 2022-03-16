#ifndef __RGL_X11_INPUT_H
#define __RGL_X11_INPUT_H

#include "../rgl_input.h"
#include "rgl_x11_context.h"

void rgl_x11_input_update(void);
void rgl_x11_input_post_update(void);

void rgl_x11_get_cursor_pos(v2 *vec);

b8 rgl_x11_is_key_pressed(rgl_key_t key);
b8 rgl_x11_is_key_just_pressed(rgl_key_t key);
b8 rgl_x11_is_key_just_released(rgl_key_t key);

b8 rgl_x11_is_btn_pressed(rgl_btn_t btn);
b8 rgl_x11_is_btn_just_pressed(rgl_btn_t btn);
b8 rgl_x11_is_btn_just_released(rgl_btn_t btn);

#endif /* __RGL_X11_INPUT_H */
