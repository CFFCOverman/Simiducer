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
    delete m_Texture;
    delete m_CameraController;
}

void EarthLayer::OnAttach() {
    Simiducer::Console::Log("[System] EarthLayer initialized successfully.");
    Simiducer::Console::Log("[System] Welcome to Simiducer Engine V3.");

    // 1. 初始化摄像机与控制器
    m_Camera = new Camera(3.5f, 0.0f, 0.0f);
    m_CameraController = new Simiducer::CameraController(m_Camera);

    // 2. 加载着色器与模型 
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
    m_CameraController->OnUpdate(0.016f);

    if (Simiducer::Input::IsKeyPressed(Simiducer::Key::Space)) {
        Simiducer::Console::Log("[Input] Space key pressed! Time machine activated!");
    }

    // ==========================================
    // 第二部分：准备渲染管线与传递数据
    // ==========================================
    m_Shader->use();

    m_Texture->Bind(0);
    m_Shader->setInt("texture1", 0);
    m_Shader->setFloat("u_Year", m_Year);

    // 摄像机矩阵
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), m_AspectRatio, 0.1f, 100.0f);
    glm::mat4 view = m_Camera->GetViewMatrix();
    m_Shader->setMat4("projection", projection);
    m_Shader->setMat4("view", view);

    // 【修改】：使用 m_EarthTransform 动态生成模型矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, m_EarthTransform.Position);
    // 按 X, Y, Z 轴顺序应用欧拉角旋转
    model = glm::rotate(model, glm::radians(m_EarthTransform.Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_EarthTransform.Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(m_EarthTransform.Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, m_EarthTransform.Scale);
    m_Shader->setMat4("model", model);

    // 【新增】：传递模块化后的太阳光数据
    m_Shader->setVec3("lightPos", m_Sun.Position);
    m_Shader->setVec3("lightColor", m_Sun.Color);
    m_Shader->setFloat("ambientStrength", m_Sun.AmbientStrength);
    m_Shader->setFloat("specularStrength", m_Sun.SpecularStrength);

    // 传递摄像机位置，用于计算高光
    m_Shader->setVec3("viewPos", m_Camera->GetPosition());

    // ==========================================
    // 第三部分：执行绘制指令
    // ==========================================
    m_Earth->draw();
}

void EarthLayer::OnUIRender() {
    // ==========================================
    // 业务逻辑面板
    // ==========================================
    ImGui::Begin("Time Machine Control");
    ImGui::Text("Engine Architecture V3: Online");
    ImGui::SliderFloat("Year", &m_Year, -2000.0f, 2026.0f);
    ImGui::End();

    Simiducer::Console::Draw("Developer Console");

    // ==========================================
    // 【新增】：属性检查器面板 (Inspector)
    // ==========================================
    ImGui::Begin("Inspector");

    ImGui::Text("--- Directional Light (Sun) ---");
    ImGui::DragFloat3("Sun Position", &m_Sun.Position.x, 0.1f);
    ImGui::ColorEdit3("Sun Color", &m_Sun.Color.x);
    ImGui::SliderFloat("Ambient", &m_Sun.AmbientStrength, 0.0f, 1.0f);
    ImGui::SliderFloat("Specular", &m_Sun.SpecularStrength, 0.0f, 1.0f);

    ImGui::Separator();

    ImGui::Text("--- Earth Transform ---");
    // 拖动此处的 Y 轴可以让地球自转
    ImGui::DragFloat3("Rotation", &m_EarthTransform.Rotation.x, 1.0f);
    ImGui::DragFloat3("Scale", &m_EarthTransform.Scale.x, 0.01f);
    ImGui::DragFloat3("Position", &m_EarthTransform.Position.x, 0.05f);

    ImGui::End();
}

void EarthLayer::OnEvent(Simiducer::Event& event) {
    Simiducer::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Simiducer::MouseScrolledEvent>(
        [this](Simiducer::MouseScrolledEvent& e) { return OnMouseScroll(e); }
    );

    dispatcher.Dispatch<Simiducer::WindowResizeEvent>(
        [this](Simiducer::WindowResizeEvent& e) { return OnWindowResize(e); }
    );
}

bool EarthLayer::OnMouseScroll(Simiducer::MouseScrolledEvent& e) {
    if (m_CameraController) {
        m_CameraController->Zoom(e.GetYOffset());
    }
    return false;
}

bool EarthLayer::OnWindowResize(Simiducer::WindowResizeEvent& e) {
    if (e.GetHeight() == 0 || e.GetWidth() == 0) {
        return false;
    }
    m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
    return false;
}