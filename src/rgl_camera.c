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
	camera->left	= -_rgl_app_desc->width / camera->zoom + camera->position.x;
	camera->top	= -_rgl_app_desc->height / camera->zoom + camera->position.y;
	camera->right	= _rgl_app_desc->width / camera->zoom + camera->position.x;
	camera->bottom	= _rgl_app_desc->height / camera->zoom + camera->position.y;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(camera->left, camera->right, camera->bottom, camera->top, -1, 1);
	rglMat4Ortho(camera->projection, camera->left, camera->right, camera->bottom, camera->top, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
