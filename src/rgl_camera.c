#include "rgl_camera.h"
#include "rgl.h"

void
rglCameraCreate(rglCamera *camera, rglV2 position, f32 rotation, f32 zoom) {
	rglV2Copy(&camera->position, &position);
	camera->rotation = rotation;
	camera->zoom = zoom;
}

void
rglCameraDestroy(rglCamera *camera) {
}

void
rglCameraUpdate(rglCamera *camera) {
	f32 hw = _rgl_data->width * 0.5f;
	f32 hh = _rgl_data->height * 0.5f;

	f32 left	= camera->position.x; //- hw;
	f32 bottom	= camera->position.y; //- hh;
	f32 right	= _rgl_data->width;// + hw;
	f32 top		= _rgl_data->height; //+ hh;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(left, right, top, bottom, -1, 1);
	rglMat4Ortho(camera->projection, left, right, top, bottom, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
