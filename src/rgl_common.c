#include "rgl_common.h"

void _rglSwap(void *a, void *b, size_t size) {
	void *temp = malloc(size);
	memcpy(temp, a, size);

	memcpy(a, b, size);
	memcpy(b, temp, size);

	free(temp);
}
