#pragma once

// 引入引擎核心与基类
#include <Simiducer/Layer.h>

// 引入事件系统
#include <Simiducer/Event.h>
#include <Simiducer/MouseEvent.h>

// 引入图形与功能组件
#include <Simiducer/Shader.h>
#include <Simiducer/Sphere.h>
#include <Simiducer/Camera.h>
#include <Simiducer/Texture.h>
#include <Simiducer/CameraController.h>
#include <Simiducer/WindowEvent.h>

class EarthLayer : public Simiducer::Layer {
public:
    EarthLayer();
    virtual ~EarthLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override {}// 补齐这个 override
    virtual void OnUpdate() override;
    virtual void OnUIRender() override;

    // 全能的事件接收接口
    virtual void OnEvent(Simiducer::Event& event) override;

private:
    // 专门用来处理鼠标滚轮的具体私有函数
    bool OnMouseScroll(Simiducer::MouseScrolledEvent& e);
    bool OnWindowResize(Simiducer::WindowResizeEvent& e);
    Shader* m_Shader;
    Sphere* m_Earth;
    Camera* m_Camera;
    Simiducer::Texture* m_Texture;
    Simiducer::CameraController* m_CameraController;


    float m_AspectRatio = 1.77778f;
    float m_Year = 2026.0f;
};