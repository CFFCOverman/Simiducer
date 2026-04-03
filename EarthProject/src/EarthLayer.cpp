#include "EarthLayer.h"
#include <imgui.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Simiducer/Input.h"
#include "Simiducer/Console.h"
EarthLayer::EarthLayer() {}

EarthLayer::~EarthLayer() {
    delete m_Shader;
    delete m_Earth;
    delete m_Camera;
    delete m_Texture;          // 【新增】防止内存泄漏
    delete m_CameraController; // 【新增】防止内存泄漏
}

void EarthLayer::OnAttach() {

    Simiducer::Console::Log("[System] EarthLayer initialized successfully.");
    Simiducer::Console::Log("[System] Welcome to Simiducer Engine V3.");
    // 1. 初始化摄像机与控制器
    m_Camera = new Camera(3.5f, 0.0f, 0.0f);
    m_CameraController = new Simiducer::CameraController(m_Camera);

    // 2. 加载着色器与模型 (继续使用绝对路径测试)
    m_Shader = new Shader("C:/Users/pinyo/source/repos/Simiducer/assets/basic.vert",
        "C:/Users/pinyo/source/repos/Simiducer/assets/basic.frag");
    m_Earth = new Sphere(1.0f, 48, 24);

    // 3. 加载地球贴图
    m_Texture = new Simiducer::Texture("C:/Users/pinyo/source/repos/Simiducer/assets/earth.jpg");
}

void EarthLayer::OnUpdate() {
    // ==========================================
    // 第一部分：处理输入与逻辑更新
    // ==========================================

    // 让控制器更新鼠标拖拽状态 (0.016f 为模拟 60帧 的时间步长)
    m_CameraController->OnUpdate(0.016f);

    // 监听空格键测试神经系统
    if (Simiducer::Input::IsKeyPressed(Simiducer::Key::Space)) {
        Simiducer::Console::Log("[Input] Space key pressed! Time machine activated!");
    }

    // ==========================================
    // 第二部分：准备渲染管线与传递数据
    // ==========================================

    m_Shader->use();

    // 绑定贴图并告诉着色器使用 0 号插槽
    m_Texture->Bind(0);
    m_Shader->setInt("texture1", 0);

    // 传递时间机器的年份
    m_Shader->setFloat("u_Year", m_Year);

    // 计算 MVP 矩阵
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), m_AspectRatio, 0.1f, 100.0f);
    glm::mat4 view = m_Camera->GetViewMatrix();

    glm::mat4 model = glm::mat4(1.0f);
    // 地球自转逻辑
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // 将矩阵传给显卡
    m_Shader->setMat4("projection", projection);
    m_Shader->setMat4("view", view);
    m_Shader->setMat4("model", model);

    // ==========================================
    // 第三部分：执行绘制指令
    // ==========================================

    m_Earth->draw();
}

void EarthLayer::OnUIRender() {
    // 绘制时间机器 UI 面板
    ImGui::Begin("Time Machine Control");
    ImGui::Text("Engine Architecture V3: Online");
    ImGui::SliderFloat("Year", &m_Year, -2000.0f, 2026.0f);
    ImGui::End();
    // 【新增】：渲染开发者控制台
    Simiducer::Console::Draw("Developer Console");
}

void EarthLayer::OnEvent(Simiducer::Event& event) {
    // 1. 创建一个事件分发器，把刚刚收到的事件包裹丢进去
    Simiducer::EventDispatcher dispatcher(event);

    // 2. 告诉分发器：如果拆出来发现是 MouseScrolledEvent，就把它交给我的 OnMouseScroll 函数处理
    // 这里使用了一个 C++ 的 Lambda 表达式作为回调桥梁
    dispatcher.Dispatch<Simiducer::MouseScrolledEvent>(
        [this](Simiducer::MouseScrolledEvent& e) { return OnMouseScroll(e); }
    );

    // 【新增】：告诉分发器，遇到窗口缩放事件，交给 OnWindowResize 处理
    dispatcher.Dispatch<Simiducer::WindowResizeEvent>(
        [this](Simiducer::WindowResizeEvent& e) { return OnWindowResize(e); }
    );
}

bool EarthLayer::OnMouseScroll(Simiducer::MouseScrolledEvent& e) {
    if (m_CameraController) {
        // 从事件包裹中取出 Y 轴的滚动偏移量，传给摄像机控制器
        m_CameraController->Zoom(e.GetYOffset());
    }
    
    // 返回 false 表示：虽然我处理了这个事件，但不要拦截它，允许它继续传给底下的其他 Layer 
    // (如果在写 UI 阻挡点击，这里就可以 return true)
    return false; 
}

bool EarthLayer::OnWindowResize(Simiducer::WindowResizeEvent& e) {
    // 防止除以 0 的崩溃（当窗口被最小化时宽度/高度可能是 0）
    if (e.GetHeight() == 0 || e.GetWidth() == 0) {
        return false;
    }

    // 更新长宽比
    m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    return false;
}