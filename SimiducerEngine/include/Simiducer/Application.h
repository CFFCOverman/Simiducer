#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Simiducer {
    class Application {
    public:
        Application();
        virtual ~Application();
        void Run();
    private:
        GLFWwindow* m_Window;
        bool m_Running = true;
    };
}