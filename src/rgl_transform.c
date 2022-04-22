#include "rgl_transform.h"

/* TODO: Optimize this shit */
void 
rglTransformCalculateModelMatrix(rglMat4 model_matrix, rglTransform *transform) {
	RGL_ASSERT_VALID_PTR(transform);

	rglMat4 translation_matrix;
	rglMat4Identity(translation_matrix);
	rglMat4Translate(translation_matrix, &transform->position);

	rglMat4 rotation_matrix;
	rglMat4Identity(rotation_matrix);
	rglMat4Rotate(rotation_matrix, transform->rotation);

	rglMat4 scale_matrix;
	rglMat4Identity(scale_matrix);
	rglMat4Scale(scale_matrix, &transform->scale);

	rglMat4Identity(model_matrix);
	rglMat4Mul(model_matrix, translation_matrix);
	rglMat4Mul(model_matrix, rotation_matrix);
	rglMat4Mul(model_matrix, scale_matrix);
}
