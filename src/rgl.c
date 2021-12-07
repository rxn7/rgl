#include <rgl/rgl.h>
#include <stdio.h>

static void _rgl_glfw_error_callback(int code, const char *msg);

rgl_app_cxt_t rgl_cxt_instance;

b8 rgl_init(rgl_app_desc_t *desc) {
        rgl_cxt_instance.app_desc = desc;

	glfwSetErrorCallback(_rgl_glfw_error_callback);
        if(!glfwInit()) {
		printf("[RGL] Failed to initialize glfw.\n");
                return false;
        }

        rgl_cxt_instance.window = glfwCreateWindow(desc->width, desc->height, desc->title, NULL, NULL);
        if(!rgl_cxt_instance.window) {
		printf("[RGL] Failed to create glfw window.\n");
                return false;
        }

	glfwMakeContextCurrent(rgl_cxt_instance.window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        f32 dt = 0, now = 0, last = 0;
        while(!glfwWindowShouldClose(rgl_cxt_instance.window)) {
                now = glfwGetTime();
                dt = (f32)(now - last);
                last = now;

                glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);

                if(rgl_cxt_instance.app_desc->update_f) {
                        rgl_cxt_instance.app_desc->update_f(dt);
                }

                glfwSwapBuffers(rgl_cxt_instance.window);
        }

        if(rgl_cxt_instance.app_desc->exit_f) {
                rgl_cxt_instance.app_desc->exit_f();
        }

        glfwDestroyWindow(rgl_cxt_instance.window);

        return true;
}

static void _rgl_glfw_error_callback(int code, const char *msg) {
	printf("[RGL] glfw error [%i]: %s\n", code, msg);
}
