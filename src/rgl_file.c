#include <stdio.h>
#include <stdlib.h>

#include "rgl_file.h"

char *rglReadFile(const char *path) {
        FILE *f = fopen(path, "rb");

        if(!f) {
                RGL_LOG_ERROR("Failed to read file `%s`!", path); 
                return "";
        }

        /* Get the file's size. */
        fseek(f, 0, SEEK_END);
        size_t sz = ftell(f);
        fseek(f, 0, SEEK_SET);

        char *buf = (char*)malloc(sz + 1);
        fread((void*)buf, 1, sz, f);
        fclose(f);
        buf[sz] = '\0';

	return buf;
}
