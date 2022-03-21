#ifndef __RGL_CAMERA_H
#define __RGL_CAMERA_H

#include "rgl_common.h"

typedef struct rglCamera {
	rglMat4 projection;
	rglV2 position;
	f32 zoom;
	f32 left, right, top, bottom;
} rglCamera;

void rglCameraCreate(rglCamera *camera, rglV2 position, f32 zoom);
void rglCameraDestroy(rglCamera *camera);
void rglCameraUpdate(rglCamera *camera);

#endif /* __RGL_CAMERA_H */
