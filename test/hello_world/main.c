#include <rgl/rgl.h>

void update(f32 dt) {
        printf("FPS: %f\n", 1.f / dt);
}

int main(int argc, const char **argv) {
        rgl_app_desc_t desc = (rgl_app_desc_t){
                .title = "Hello World!",
                .width = 960,
                .height = 640,
                .update_f = update,
                .exit_f = 0,
        };

        rgl_init(&desc);
}
