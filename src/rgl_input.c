#include "rgl_common.h"
#include "rgl_input.h"

void rgl_get_cursor_pos(f64 *x, f64 *y) {
        *x = _app_data.mouse_x;
        *y = _app_data.mouse_y;
}

bool rgl_is_key_pressed(s32 key) {
        return glfwGetKey(_app_data.window, key) == GLFW_PRESS;
}

bool rgl_is_button_pressed(s32 btn) {
        return glfwGetMouseButton(_app_data.window, btn) == GLFW_PRESS;
}
