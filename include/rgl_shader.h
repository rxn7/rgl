#ifndef __RGL_SHADER_H
#define __RGL_SHADER_H

#include "rgl_common.h"

typedef struct rglShader {
	u32 id;
	u32 *uniform_locations;
} rglShader;

b8 rgl_shader_create(rglShader *shader, const char *vert_src, const char *frag_src);
void rgl_shader_destroy(rglShader *shader);

void _rgl_shader_create_defaults();
void _rgl_shader_destroy_defaults();
b8 _rgl_shader_init(u32 *shader, u32 type, const char *src);

#endif /* __RGL_SHADER_H */
