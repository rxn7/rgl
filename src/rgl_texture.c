#include <stdlib.h>
#include <string.h>

#include "rgl.h"
#include "rgl_texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

static void _apply_parameters(u8 filter) {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	switch(filter) {
		default:
		case RGL_TEXTURE_FILTER_NONE:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;

		case RGL_TEXTURE_FILTER_LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
	}
}

b8 rgl_texture_create(rgl_texture_t *txt, i32 w, i32 h, u8 *pixels, u8 filter) {
	RGL_ASSERT_VALID_PTR(txt);
	RGL_ASSERT_VALID_PTR(pixels);

        txt->width = w;
        txt->height = h;

	glGenTextures(1, &txt->id);
	glBindTexture(GL_TEXTURE_2D, txt->id);
	_apply_parameters(filter);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

b8 rgl_texture_load_from_file(rgl_texture_t *txt, const char *path, u8 filter) {
	RGL_ASSERT_VALID_PTR(txt);

	i32 channels;
	stbi_set_flip_vertically_on_load(false);
	u8 *pixels = stbi_load(path, &txt->width, &txt->height, &channels, STBI_rgb_alpha);

	RGL_ASSERT_VALID_PTR(pixels);

	glGenTextures(1, &txt->id);
	glBindTexture(GL_TEXTURE_2D, txt->id);
	_apply_parameters(filter);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, txt->width, txt->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(pixels);

	return true;
}

void rgl_texture_destroy(rgl_texture_t *txt) {
	RGL_ASSERT_VALID_PTR(txt);
	glDeleteTextures(1, &txt->id);
}
