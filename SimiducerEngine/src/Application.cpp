#include "Simiducer/Application.h"
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include "Simiducer/MouseEvent.h"
#include "Simiducer/WindowEvent.h"
#include "Simiducer/Console.h"

GLFWwindow* g_Window = nullptr;

namespace Simiducer {
    Application::Application() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(1280, 720, "Simiducer Engine V3.0", nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        g_Window = m_Window;

        glfwSetWindowUserPointer(m_Window, this);

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
            auto& app = *(Application*)glfwGetWindowUserPointer(window);
            MouseScrolledEvent event((float)xoffset, (float)yoffset);
            for (auto layer : app.m_LayerStack) {
                layer->OnEvent(event);
                if (event.Handled) break;
            }
            });

        // 【修改】：操作系统窗口拉伸时，我们不再发送事件，只是告诉 OpenGL 适应外壳
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            glViewport(0, 0, width, height);
            });

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        glEnable(GL_DEPTH_TEST);
        InitImGui();

        // 【新增】：初始化隐形画布
        m_Framebuffer = new Framebuffer(1280, 720);
    }

    Application::~Application() {
        delete m_Framebuffer; // 【新增】：释放内存
        ShutdownImGui();
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.push_back(layer);
        layer->OnAttach();
    }

    void Application::Run() {
        while (m_Running && !glfwWindowShouldClose(m_Window)) {
            // ====== 【新增：清空主系统屏幕，解决 UI 拖影！】 ======
            glBindFramebuffer(GL_FRAMEBUFFER, 0); // 确保切回主屏幕
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // 主屏幕底色设为纯黑
            glClear(GL_COLOR_BUFFER_BIT);
            // ==================================================

            // 1. 第一阶段：离屏渲染 (画到画布上)
            m_Framebuffer->Bind();
            glClearColor(0.02f, 0.02f, 0.04f, 1.0f); // 画布的宇宙深蓝色
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // ==========================================
            // 第一阶段：离屏渲染 (画到画布上)
            // ==========================================
            m_Framebuffer->Bind();
            glClearColor(0.02f, 0.02f, 0.04f, 1.0f); // 宇宙背景色
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate(); // 让所有图层开始画画
            }
            m_Framebuffer->Unbind();

            // ==========================================
            // 第二阶段：UI 渲染 (把画布贴出来)
            // ==========================================
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 画业务层的 UI (时间机器面板)
            for (Layer* layer : m_LayerStack) {
                layer->OnUIRender();
            }

            // 画引擎的工具面板
            Simiducer::Console::Draw("Developer Console");
            m_ContentBrowser.OnUIRender();

            // 【核心重构】：渲染 Viewport
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
            ImGui::Begin("Viewport");

            ImVec2 viewportSize = ImGui::GetContentRegionAvail();

            // 【神来之笔】：如果用户拖拉了 Viewport 窗口，我们在这里生成缩放事件！
            if (m_ViewportWidth != (uint32_t)viewportSize.x || m_ViewportHeight != (uint32_t)viewportSize.y) {
                m_ViewportWidth = (uint32_t)viewportSize.x;
                m_ViewportHeight = (uint32_t)viewportSize.y;

                m_Framebuffer->Resize(m_ViewportWidth, m_ViewportHeight);

                // 发送给 EarthLayer，让它更新摄像机的长宽比
                WindowResizeEvent event(m_ViewportWidth, m_ViewportHeight);
                for (auto layer : m_LayerStack) {
                    layer->OnEvent(event);
                }
            }

            // 把显卡内存里的画布拿出来贴上
            uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
            ImGui::Image((void*)(intptr_t)textureID, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

            ImGui::End();
            ImGui::PopStyleVar();

            // 合并输出
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            // ====== 【新增：接管飞出主屏幕的独立小窗口】 ======
            ImGuiIO& io = ImGui::GetIO();
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                // 1. 记住当前的主屏幕上下文
                GLFWwindow* backup_current_context = glfwGetCurrentContext();

                // 2. 让 ImGui 自动去渲染那些飞出去的独立窗口
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();

                // 3. 渲染完子窗口后，务必把上下文切回我们的主屏幕！
                glfwMakeContextCurrent(backup_current_context);
            }
            // =================================================
            glfwSwapBuffers(m_Window);
            glfwPollEvents();
        }
    }

    void Application::InitImGui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // ====== 【新增：获取 IO 对象并开启多视口与停靠特性】 ======
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // 允许面板吸附合并
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // 允许面板拖出主窗口
        // ==========================================================

        ImGui::StyleColorsDark();

        // 如果开启了多视口，稍微调整一下脱离出来的窗口样式，让它更像原生窗口
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }
    void Application::ShutdownImGui() { /* 保持原样 */
        ImGui_ImplOpenGL3_Shutdown(); ImGui_ImplGlfw_Shutdown(); ImGui::DestroyContext();
    }
}