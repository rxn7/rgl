#include "rgl_mat4.h"

/* TODO: Should I assert the parameters in these functions? */

void rglMat4Cpy(rglMat4 src, rglMat4 dest) {
	for(u8 i=0; i<16; ++i) {
		dest[i] = src[i];
	}
}

void rglMat4Identity(rglMat4 mat) {
	memset(mat, 0, sizeof(f32) * 16);
	mat[0] = mat[5] = mat[10] = mat[15] = 1.0f;
}

void rglMat4Ortho(rglMat4 mat, f32 left, f32 right, f32 bottom, f32 top, f32 z_near, f32 z_far) {
	rglMat4Identity(mat);
	mat[0] = 2.f / (right - left);
	mat[5] = 2.f / (top - bottom);
	mat[10] = - 2.f / (z_far - z_near);
	mat[12] = - (right + left) / (right - left);
	mat[13] = - (top + bottom) / (top - bottom);
	mat[14] = - (z_far + z_near) / (z_far - z_near);
	mat[15] = 1;
}

void rglMat4Mul(rglMat4 a, rglMat4 b) {
	rglMat4 ac;
	rglMat4Cpy(a, ac);

	a[0] = ac[0] * b[0] + ac[4] * b[1] + ac[8] * b[2] + ac[12] * b[3];
	a[1] = ac[1] * b[0] + ac[5] * b[1] + ac[9] * b[2] + ac[13] * b[3];
	a[2] = ac[2] * b[0] + ac[6] * b[1] + ac[10] * b[2] + ac[14] * b[3];
	a[3] = ac[3] * b[0] + ac[7] * b[1] + ac[11] * b[2] + ac[15] * b[3];

	a[4] = ac[0] * b[4] + ac[4] * b[5] + ac[8] * b[6] + ac[12] * b[7];
	a[5] = ac[1] * b[4] + ac[5] * b[5] + ac[9] * b[6] + ac[13] * b[7];
	a[6] = ac[2] * b[4] + ac[6] * b[5] + ac[10] * b[6] + ac[14] * b[7];
	a[7] = ac[3] * b[4] + ac[7] * b[5] + ac[11] * b[6] + ac[15] * b[7];

	a[8] = ac[0] * b[8] + ac[4] * b[9] + ac[8] * b[10] + ac[12] * b[11];
	a[9] = ac[1] * b[8] + ac[5] * b[9] + ac[9] * b[10] + ac[13] * b[11];
	a[10] = ac[2] * b[8] + ac[6] * b[9] + ac[10] * b[10] + ac[14] * b[11];
	a[11] = ac[3] * b[8] + ac[7] * b[9] + ac[11] * b[10] + ac[15] * b[11];

	a[12] = ac[0] * b[12] + ac[4] * b[13] + ac[8] * b[14] + ac[12] * b[15];
	a[13] = ac[1] * b[12] + ac[5] * b[13] + ac[9] * b[14] + ac[13] * b[15];
	a[14] = ac[2] * b[12] + ac[6] * b[13] + ac[10] * b[14] + ac[14] * b[15];
	a[15] = ac[3] * b[12] + ac[7] * b[13] + ac[11] * b[14] + ac[15] * b[15];
}

void rglMat4Translate(rglMat4 mat, v2 vec) {
	memset(mat, 0, sizeof(f32) * 16);

	mat[0] = mat[5] = mat[10] = 1.0f; 

	mat[12] = vec.x;
	mat[13] = vec.y;
	mat[14] = 1.0f;
	mat[15] = 1.0f;
}

void rglMat4Rotate(rglMat4 mat, f32 degrees) {
	f32 rad = degrees * (PI_F / 180.f);
	f32 radcos = cosf(rad);
	f32 radsin = sinf(rad);

	mat[0] = radcos;
	mat[1] = radsin;
	mat[2] = 0.0f;
	mat[3] = 0.0f;

	mat[4] = -radsin;
	mat[5] = radcos;
	mat[5] = radcos;
	mat[6] = 0.0f;

	mat[8] = 0.0f;
	mat[9] = 0.0f;
	mat[10] = 1.0f;
	mat[11] = 0.0f;

	mat[12] = 0.0f;
	mat[13] = 0.0f;
	mat[14] = 0.0f;
	mat[15] = 1.0f;
}

void rglMat4Scale(rglMat4 mat, v2 vec) {
	memset(mat, 0, sizeof(f32) * 16);

	mat[0] = vec.x;
	mat[5] = vec.y;
	mat[10] = 1.0f;
	mat[15] = 1.0f;
}
