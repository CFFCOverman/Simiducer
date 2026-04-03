#include "EarthLayer.h"
#include <imgui.h>

EarthLayer::EarthLayer() {}

EarthLayer::~EarthLayer() {
    delete m_Shader;
    delete m_Earth;
    delete m_Camera;
}

void EarthLayer::OnAttach() {

    m_Camera = new Camera(0.0f, 0.0f, 3.0f);

    // 【修改为绝对路径测试】
    m_Shader = new Shader("C:/Users/pinyo/source/repos/Simiducer/assets/basic.vert",
        "C:/Users/pinyo/source/repos/Simiducer/assets/basic.frag");

    m_Earth = new Sphere(1.0f, 48, 24);

    // 【修改为绝对路径测试】
    m_Texture = new Simiducer::Texture("C:/Users/pinyo/source/repos/Simiducer/assets/earth.jpg");

   /* m_Camera = new Camera(0.0f, 0.0f, 3.0f);
    m_Shader = new Shader("../assets/basic.vert", "../assets/basic.frag");
    m_Earth = new Sphere(1.0f, 48, 24);

    // 【新增】：加载地球贴图
    m_Texture = new Simiducer::Texture("../assets/earth.jpg");
    */
}

void EarthLayer::OnUpdate() {
    m_Shader->use();
    m_Shader->setFloat("u_Year", m_Year);

    // ====== 【新增的输入测试】 ======
      // 如果按下空格键，就在终端打印一句话
    if (Simiducer::Input::IsKeyPressed(Simiducer::Key::Space)) {
        std::cout << "Space key pressed! Time machine activated!" << std::endl;
    }
    // ===============================

    // ... 后面画地球的代码保持不变 ...

    // 【新增】：绑定贴图，并告诉着色器使用 0 号插槽
    m_Texture->Bind();
    m_Shader->setInt("texture1", 0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    glm::mat4 view = m_Camera->GetViewMatrix();
    m_Shader->setMat4("projection", projection);
    m_Shader->setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(10.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Shader->setMat4("model", model);

    // 【删除这行】：glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    m_Earth->draw();
    // 【删除这行】：glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void EarthLayer::OnUIRender() {
    // 绘制时间机器 UI 面板
    ImGui::Begin("Time Machine Control");
    ImGui::Text("Engine Architecture V3: Online");
    ImGui::SliderFloat("Year", &m_Year, -2000.0f, 2026.0f);
    ImGui::End();
}