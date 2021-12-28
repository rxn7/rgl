#pragma once

#include <stdio.h>

#include "glad.h"
#include "rgl_common.h"
#include "rgl_color.h"
#include "rgl_texture.h"

b8 rgl_init(rgl_app_desc_t *desc);
void rgl_get_window_size(u32 *w, u32 *h);

void rgl_render_texture(rgl_texture_t *txt, b8 stretch);
