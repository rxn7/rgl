#include <rgl/rgl.h>

int main(int argc, const char **argv) {
        rgl_app_desc_t desc = (rgl_app_desc_t){
                .title = "Hello World!",
                .width = 960,
                .height = 640,
                .update_f = 0,
                .exit_f = 0,
        };

        rgl_init(&desc);
}
