#include <rgl.h>
#include <GLFW/glfw3.h> /* TODO: Don't use GLFW */

rgl_app_cxt_t rgl_cxt_instance;

b8 rgl_init(rgl_app_desc_t *desc) {
        rgl_cxt_instance.app_desc = desc;

        if(!glfwInit()) {
                return false;
        }

        rgl_cxt_instance.window = glfwCreateWindow(desc->width, desc->height, desc->title, NULL, NULL);
        if(!rgl_cxt_instance.window) {
                return false;
        }

        while(!glfwWindowShouldClose(rgl_cxt_instance.window)) {
                glfwPollEvents();
                glClear(GL_COLOR_BUFFER_BIT);

                if(rgl_cxt_instance.app_desc->update_f) {
                        rgl_cxt_instance.app_desc->update_f(0);
                }

                glfwSwapBuffers(rgl_cxt_instance.window);
        }

        if(rgl_cxt_instance.app_desc->exit_f) {
                rgl_cxt_instance.app_desc->exit_f();
        }

        glfwDestroyWindow(rgl_cxt_instance.window);

        return true;
}
