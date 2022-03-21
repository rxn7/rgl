#ifndef __RGL_X11_INPUT_H
#define __RGL_X11_INPUT_H

#include "../rgl_input.h"
#include "rgl_x11_context.h"

void rglX11InputUpdate(void);
void rglX11InputPostUpdate(void);

void rglX11GetCursorPos(rglV2 *vec);

b8 rglX11IsKeyPressed(rglKey key);
b8 rglX11IsKeyJustPressed(rglKey key);
b8 rglX11IsKeyJustReleased(rglKey key);

b8 rglX11IsBtnPressed(rglBtn btn);
b8 rglX11IsBtnJustPressed(rglBtn btn);
b8 rglX11IsBtnJustReleased(rglBtn btn);

#endif /* __RGL_X11_INPUT_H */
