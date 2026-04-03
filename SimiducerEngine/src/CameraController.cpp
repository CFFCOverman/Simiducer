#include "Simiducer/CameraController.h"
#include <GLFW/glfw3.h>

// 拿到刚才在 Input 系统里设置好的全局窗口指针
extern GLFWwindow* g_Window;

namespace Simiducer {
    CameraController::CameraController(Camera* camera) : m_Camera(camera) {}

    void CameraController::OnUpdate(float dt) {
        // 用于记录上一帧的鼠标位置
        static double lastX = 0.0, lastY = 0.0;
        static bool firstMouse = true;

        double xpos, ypos;
        glfwGetCursorPos(g_Window, &xpos, &ypos);

        // 只有当鼠标【左键】按住时，才允许旋转地球
        if (glfwGetMouseButton(g_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            if (firstMouse) {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }

            float xoffset = (float)(xpos - lastX);
            float yoffset = (float)(lastY - ypos); // Y坐标是反的，所以用 last - current

            lastX = xpos;
            lastY = ypos;

            // 直接调用你写好的摄像机旋转函数！
            m_Camera->ProcessMouseMovement(xoffset, yoffset);
        }
        else {
            // 松开左键时重置状态
            firstMouse = true;
        }
    }

    void CameraController::Zoom(float yoffset) {
        // 调用你写好的滚轮缩放函数！
        m_Camera->ProcessMouseScroll(yoffset);
    }
}