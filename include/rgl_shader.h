#ifndef __RGL_SHADER_H
#define __RGL_SHADER_H

#include "rgl_common.h"

typedef struct rgl_shader_t {
	u32 id;
	u32 *uniform_locations;
} rgl_shader_t;

b8 rgl_shader_create(rgl_shader_t *shader, const char *vert_src, const char *frag_src);
void rgl_shader_destroy(rgl_shader_t *shader);

void _rgl_shader_create_defaults();
void _rgl_shader_destroy_defaults();
b8 _rgl_shader_init(u32 *shader, u32 type, const char *src);

#endif /* __RGL_SHADER_H */
