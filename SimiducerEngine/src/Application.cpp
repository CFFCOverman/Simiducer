#include "Simiducer/Application.h"
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace Simiducer {
    Application::Application() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(1280, 720, "Simiducer Engine V3.0", nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        // 开启深度测试，防止地球背面的顶点渲染到前面
        glEnable(GL_DEPTH_TEST);

        InitImGui();
        std::cout << "Engine Initialized! OpenGL: " << glGetString(GL_VERSION) << std::endl;
    }

    Application::~Application() {
        ShutdownImGui();
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack.push_back(layer);
        layer->OnAttach(); // 塞进来的时候，立刻执行初始化
    }

    void Application::Run() {
        while (m_Running && !glfwWindowShouldClose(m_Window)) {
            // 清除屏幕和深度缓冲
            glClearColor(0.02f, 0.02f, 0.04f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // 1. 让所有的图层更新自己的 3D 画面
            for (Layer* layer : m_LayerStack) {
                layer->OnUpdate();
            }

            // 2. 准备 UI 渲染环境
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // 3. 让所有的图层画自己的 UI 面板
            for (Layer* layer : m_LayerStack) {
                layer->OnUIRender();
            }

            // 4. 合并并输出画面
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(m_Window);
            glfwPollEvents();
        }
    }

    void Application::InitImGui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void Application::ShutdownImGui() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}