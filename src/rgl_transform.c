#include "rgl_transform.h"

void rgl_transform_initialize(rgl_transform_t *trans) {
	trans->rotation = 0;
	glm_vec2_copy((vec2){0, 0}, trans->position);
	glm_vec2_copy((vec2){1, 1}, trans->scale);
}

void rgl_transform_calculate(mat4 dest, rgl_transform_t *trans) {
	glm_mat4_identity(dest);
	glm_translate(dest, trans->position);
	glm_rotate(dest, trans->rotation, (vec3){0,0,1});
	glm_scale(dest, trans->scale);
}
