#include <stdio.h>
#include <stdlib.h>

#include "rgl_file.h"

void rgl_read_file(char **buf, const char *path) {
	RGL_ASSERT(buf, false);

        FILE *f = fopen(path, "rb");

        if(!f) {
                RGL_LOG_ERROR("Failed to read file `%s`!", path); 
                return;
        }

        /* Get the file's size. */
        fseek(f, 0, SEEK_END);
        size_t sz = ftell(f);
        fseek(f, 0, SEEK_SET);

        *buf = (char*) malloc(sz + 1);
        fread(*buf, 1, sz, f);
        fclose(f);
        *buf[sz] = '\0';
}
