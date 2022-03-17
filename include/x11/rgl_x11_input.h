#ifndef __RGL_X11_INPUT_H
#define __RGL_X11_INPUT_H

#include "../rgl_input.h"
#include "rgl_x11_context.h"

void rgl_x11_input_update(void);
void rgl_x11_input_post_update(void);

void rgl_x11_get_cursor_pos(v2 *vec);

b8 rgl_x11_is_key_pressed(rglKey key);
b8 rgl_x11_is_key_just_pressed(rglKey key);
b8 rgl_x11_is_key_just_released(rglKey key);

b8 rgl_x11_is_btn_pressed(rglBtn btn);
b8 rgl_x11_is_btn_just_pressed(rglBtn btn);
b8 rgl_x11_is_btn_just_released(rglBtn btn);

#endif /* __RGL_X11_INPUT_H */
