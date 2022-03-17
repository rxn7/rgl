#ifndef __RGL_SHADER_H
#define __RGL_SHADER_H

#include "rgl_common.h"

typedef struct rglShader {
	u32 id;
	u32 *uniform_locations;
} rglShader;

b8 rglShaderCreate(rglShader *shader, const char *vert_src, const char *frag_src);
void rglShaderDestroy(rglShader *shader);

void _rglShaderCreateDefaults();
void _rglShaderDestroyDefaults();
b8 _rglShaderInit(u32 *shader, u32 type, const char *src);

#endif /* __RGL_SHADER_H */
