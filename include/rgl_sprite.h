#ifndef __RGL_SPRITE_H
#define __RGL_SPRITE_H

#include "rgl_vao.h"
#include "rgl_texture.h"

typedef struct rglSprite {
	rglV2 position;
	rglV2 size;
	rglV2 uvs[4];
	f32 rotation;	
	rglVao vao;
	rglTexture *texture;
} rglSprite;

b8 rglSpriteCreate(rglSprite *sprite, rglTexture *texture);
void rglSpriteDestroy(rglSprite *sprite);
void rglSpriteRender(rglSprite *sprite);
void rglSpriteSetUVs(rglSprite *sprite, rglV2 *uvs);

void _rglSpriteShaderCreate(void);
void _rglSpriteShaderDestroy(void);

#endif /* __RGL_SPRITE_H */
