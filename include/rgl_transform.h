#ifndef __RGL_TRANSFORM
#define __RGL_TRANSFORM

#include  "rgl_common.h"

typedef struct rglTransform {
	rglV2 position;
	rglV2 scale;
	f32 rotation;
} rglTransform;

void rglTransformCalculateModelMatrix(rglMat4 model_matrix, rglTransform *trans);

#endif /* __RGL_TRANSFORM */
