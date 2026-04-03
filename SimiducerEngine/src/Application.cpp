#include "Simiducer/Application.h"
#include <iostream>

namespace Simiducer {
    Application::Application() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(1280, 720, "Simiducer Engine V3.0", nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        std::cout << "Engine Initialized! OpenGL: " << glGetString(GL_VERSION) << std::endl;
    }

    Application::~Application() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Application::Run() {
        while (m_Running && !glfwWindowShouldClose(m_Window)) {
            glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // 深灰色背景
            glClear(GL_COLOR_BUFFER_BIT);

            glfwSwapBuffers(m_Window);
            glfwPollEvents();
        }
    }
}