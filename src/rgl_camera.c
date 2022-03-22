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
	f32 height_multiplier = (f32)_rgl_data->height / _rgl_data->width;

	camera->left	= -500 * camera->zoom + camera->position.x;
	camera->top	= -500 * camera->zoom * height_multiplier + camera->position.y;
	camera->right	= 500 * camera->zoom + camera->position.x;
	camera->bottom	= 500 * camera->zoom * height_multiplier + camera->position.y;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(camera->left, camera->right, camera->bottom, camera->top, -1, 1);
	rglMat4Ortho(camera->projection, camera->left, camera->right, camera->bottom, camera->top, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
