#include <stdlib.h>
#include <string.h>

#include "rgl.h"
#include "rgl_texture.h"

void rgl_texture_initialize(rgl_texture_t *txt, u32 w, u32 h) {
        txt->width = w;
        txt->height = h;
        txt->pixels = calloc(w * h, sizeof(rgl_color_t));

	glGenTextures(1, &txt->id);
	glBindTexture(GL_TEXTURE_2D, txt->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, txt->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void rgl_texture_destroy(rgl_texture_t *txt) {
	glDeleteTextures(1, &txt->id);
        free(txt->pixels);
}

void rgl_texture_update(rgl_texture_t *txt) {
	glBindTexture(GL_TEXTURE_2D, txt->id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, txt->width, txt->height, GL_RGB, GL_UNSIGNED_BYTE, txt->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}
