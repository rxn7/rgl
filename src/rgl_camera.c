#include "rgl_camera.h"
#include "rgl.h"

void
rglCameraCreate(rglCamera *camera, rglV2 position, f32 zoom) {
	rglV2Copy(&camera->position, &position);
	camera->zoom = zoom;
}

void
rglCameraDestroy(rglCamera *camera) {
}

void
rglCameraUpdate(rglCamera *camera) {
	f32 width_multiplier = (f32)_rgl_data->width / _rgl_data->height;

	camera->left	= -500 * camera->zoom * width_multiplier + camera->position.x;
	camera->bottom	= -500 * camera->zoom + camera->position.y;
	camera->right	= 500 * camera->zoom * width_multiplier + camera->position.x;
	camera->top	= 500 * camera->zoom + camera->position.y;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(camera->left, camera->right, camera->top, camera->bottom, -1, 1);
	rglMat4Ortho(camera->projection, camera->left, camera->right, camera->top, camera->bottom, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
