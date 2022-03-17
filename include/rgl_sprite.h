#ifndef __RGL_SPRITE_H
#define __RGL_SPRITE_H

#include "rgl_vao.h"
#include "rgl_texture.h"

typedef struct rglSprite {
	v2 position;
	v2 size;
	f32 rotation;	
	rglVao vao;
	rglTexture *texture;
} rglSprite;

b8 rgl_sprite_create(rglSprite *sprite, rglTexture *texture);
void rgl_sprite_destroy(rglSprite *sprite);
void rgl_sprite_render(rglSprite *sprite);

void _rgl_sprite_shader_create(void);
void _rgl_sprite_shader_destroy(void);

#endif /* __RGL_SPRITE_H */
