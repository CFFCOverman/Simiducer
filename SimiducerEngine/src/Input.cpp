#include "Simiducer/Input.h"
#include "Simiducer/Application.h"
#include <GLFW/glfw3.h>

// 这是一个极其经典的做法：通过 extern 拿到外部的全局单例窗口指针
extern GLFWwindow* g_Window;

namespace Simiducer {
    bool Input::IsKeyPressed(Key keycode) {
        // 让底层的 GLFW 去查状态
        auto state = glfwGetKey(g_Window, static_cast<int>(keycode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }
}