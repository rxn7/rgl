#include "rgl_vbo.h"
#include "rgl_texture.h"

typedef struct rgl_sprite_t {
	rgl_vbo_t vbo;
	rgl_texture_t *texture;
} rgl_sprite_t;

void rgl_sprite_initialize(rgl_sprite_t *sprite, rgl_texture_t *texture);
void rgl_sprite_destroy(rgl_sprite_t *sprite);
void rgl_sprite_render(rgl_sprite_t *sprite);
