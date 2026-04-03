#pragma once
#include "Simiducer/Layer.h"
#include "Simiducer/Event.h"
#include "Simiducer/MouseEvent.h"
#include "Simiducer/WindowEvent.h"
#include "Simiducer/Camera.h"
#include "Simiducer/CameraController.h"
#include "Simiducer/Shader.h"
#include "Simiducer/Sphere.h"
#include "Simiducer/Texture.h"
#include <glm/glm.hpp>

// ==========================================
// 【新增】：模块化数据组件
// ==========================================
struct TransformComponent {
    glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
    glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f }; // 欧拉角 (度数)
    glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
};

struct DirectionalLight {
    glm::vec3 Position = { 5.0f, 0.0f, 10.0f };
    glm::vec3 Color = { 1.0f, 1.0f, 1.0f };
    float AmbientStrength = 0.05f;
    float SpecularStrength = 0.6f;
};
// ==========================================

class EarthLayer : public Simiducer::Layer {
public:
    EarthLayer();
    virtual ~EarthLayer();

    virtual void OnAttach() override;
    virtual void OnUpdate() override;
    virtual void OnUIRender() override;
    virtual void OnEvent(Simiducer::Event& event) override;

private:
    bool OnMouseScroll(Simiducer::MouseScrolledEvent& e);
    bool OnWindowResize(Simiducer::WindowResizeEvent& e);

    Shader* m_Shader;
    Sphere* m_Earth;
    Camera* m_Camera;
    Simiducer::CameraController* m_CameraController;
    Simiducer::Texture* m_Texture;

    float m_AspectRatio = 1.77778f;
    float m_Year = 2026.0f;

    // 【新增】：实例化地球和太阳的数据组件
    TransformComponent m_EarthTransform;
    DirectionalLight m_Sun;
};