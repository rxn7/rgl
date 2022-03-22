#include "rgl_animation_texture.h"

void
rglAnimationTextureCreate(rglAnimationTexture *anim_texture, rglTexture *texture, rglV2 frame_size) {
	RGL_ASSERT_VALID_PTR(anim_texture);
	RGL_ASSERT_VALID_PTR(texture);

	rglV2Copy(&frame_size, &anim_texture->frame_size);

	anim_texture->frame_size_normalized.x = frame_size.x / texture->width;
	anim_texture->frame_size_normalized.y = frame_size.y / texture->height;

	anim_texture->cols = texture->width / frame_size.x;
}

void
rglAnimationTextureDestroy(rglAnimationTexture *anim_texture) {
	RGL_ASSERT_VALID_PTR(anim_texture);
}
