#include <stdlib.h>
#include <string.h>

#include "rgl.h"
#include "rgl_texture.h"

static void _apply_parameters();

b8 rgl_texture_create(rgl_texture_t *txt, u32 w, u32 h) {
	if(!txt) {
		RGL_LOG_ERROR("Can't create texture with null pointer");
		return false;
	}

        txt->width = w;
        txt->height = h;
        txt->pixels = calloc(w * h, sizeof(rgl_color_t));

	if(!txt->pixels) {
		RGL_LOG_ERROR("Failed to allocate pixels for texture");
		return false;
	}

	glGenTextures(1, &txt->id);
	glBindTexture(GL_TEXTURE_2D, txt->id);
	_apply_parameters();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, txt->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

b8 rgl_texture_create_from_pixels(rgl_texture_t *txt, u32 w, u32 h, u8 *pixels) {
	if(!txt) {
		RGL_LOG_ERROR("Can't create texture with null pointer");
		return false;
	}

        txt->width = w;
        txt->height = h;
        memcpy(txt->pixels, pixels, w*h);

	if(!txt->pixels) {
		RGL_LOG_ERROR("Failed to allocate pixels for texture");
		return false;
	}

	glGenTextures(1, &txt->id);
	glBindTexture(GL_TEXTURE_2D, txt->id);
	_apply_parameters();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, txt->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

b8 rgl_texture_create_from_bmp(rgl_texture_t *txt, const char *bmp_file) {
	if(!txt) {
		RGL_LOG_ERROR("Can't create texture with null pointer");
		return false;
	}

	FILE *f = fopen(bmp_file, "rb");
	if(!f) {
		RGL_LOG_ERROR("Failed to open bmp file: %s\n", bmp_file);
		return false;
	}

	u8 info[54];
	fread(info, sizeof(u8), 54, f); /* Read the 54-byte header */

	memcpy(&txt->width, info + 18, sizeof(u32));
	memcpy(&txt->height, info + 22, sizeof(u32));

	u32 size = 3 * txt->width * txt->height;

	txt->pixels = malloc(sizeof(u8) * size);
	if(!txt->pixels) {
		RGL_LOG_ERROR("Failed to allocate pixels for texture");
		return false;
	}
	
	fread(txt->pixels, sizeof(u8), size, f);
	fclose(f);

	glGenTextures(1, &txt->id);
	glBindTexture(GL_TEXTURE_2D, txt->id);
	_apply_parameters();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, txt->width, txt->height, 0, GL_RGB, GL_UNSIGNED_BYTE, txt->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

b8 rgl_texture_destroy(rgl_texture_t *txt) {
	if(!txt) {
		RGL_LOG_ERROR("Can't destroy texture with null pointer");
		return false;
	}

	glDeleteTextures(1, &txt->id);
        free(txt->pixels);

	return true;
}

void rgl_texture_update(rgl_texture_t *txt) {
	glBindTexture(GL_TEXTURE_2D, txt->id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, txt->width, txt->height, GL_RGB, GL_UNSIGNED_BYTE, txt->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
}

static void _apply_parameters() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
