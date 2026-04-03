#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Simiducer/Layer.h"

namespace Simiducer {
    class Application {
    public:
        Application();
        virtual ~Application();

        void Run();
        // 对外提供的插槽：允许外部把写好的逻辑塞进引擎
        void PushLayer(Layer* layer);

    private:
        void InitImGui();
        void ShutdownImGui();

        GLFWwindow* m_Window;
        bool m_Running = true;
        std::vector<Layer*> m_LayerStack; // 存放所有图层的容器
    };
}