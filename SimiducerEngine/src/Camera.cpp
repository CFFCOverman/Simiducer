#include "Simiducer/Camera.h"
#include <cmath>

Camera::Camera(float radius, float yaw, float pitch)
    : Radius(radius), Yaw(yaw), Pitch(pitch), Target(0.0f, 0.0f, 0.0f) {
}

glm::mat4 Camera::GetViewMatrix() const {
    // 球面坐标系转换数学
    float camX = Radius * cosf(Pitch) * sinf(Yaw);
    float camY = Radius * sinf(Pitch);
    float camZ = Radius * cosf(Pitch) * cosf(Yaw);

    glm::vec3 position = glm::vec3(camX, camY, camZ) + Target;

    // 生成并返回 View 矩阵 (摄像机位置，看向目标，Y轴朝上)
    return glm::lookAt(position, Target, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, float sensitivity) {
    Yaw += xoffset * sensitivity;
    Pitch += yoffset * sensitivity;

    // 限制俯仰角，防止镜头翻转
    if (Pitch > 1.5f) Pitch = 1.5f;
    if (Pitch < -1.5f) Pitch = -1.5f;
}

void Camera::ProcessMouseScroll(float yoffset) {
    Radius -= yoffset * 0.2f;

    // 限制缩放范围
    if (Radius < 1.2f) Radius = 1.2f;
    if (Radius > 10.0f) Radius = 10.0f;
}