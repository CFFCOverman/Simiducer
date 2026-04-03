#pragma once
#include <Simiducer.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class EarthLayer : public Simiducer::Layer {
public:
    EarthLayer();
    ~EarthLayer();

    virtual void OnAttach() override;
    virtual void OnUpdate() override;
    virtual void OnUIRender() override;

private:
    Shader* m_Shader;
    Sphere* m_Earth;
    Camera* m_Camera;
    // 在 private: 区域加上这一行
    Simiducer::Texture* m_Texture;

    float m_Year = 2026.0f; // 你的时间机器变量
};