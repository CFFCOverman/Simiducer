#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Simiducer/Layer.h"
#include "Simiducer/ContentBrowserPanel.h" // 【新增】：引入内容浏览器
#include "Simiducer/Framebuffer.h" // 【新增】：引入画布

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
        ContentBrowserPanel m_ContentBrowser;
   

        // ====== 【新增】：总导演的法宝 ======
        Framebuffer* m_Framebuffer;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
        // ====================================
    };
}