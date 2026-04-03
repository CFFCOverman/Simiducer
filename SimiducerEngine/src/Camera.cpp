#include "Simiducer/Camera.h" // 确保这里是你正确的路径
#include <cmath>

Camera::Camera(float radius, float yaw, float pitch)
    : Radius(radius), Yaw(yaw), Pitch(pitch), Target(0.0f, 0.0f, 0.0f) {
}

// 【修复】：必须加上 Camera:: 作用域！
glm::vec3 Camera::GetPosition() const {
    glm::vec3 pos;
    // 统一使用 <cmath> 里的 cosf 和 sinf 处理浮点数
    pos.x = Target.x + Radius * cosf(Pitch) * sinf(Yaw);
    pos.y = Target.y + Radius * sinf(Pitch);
    pos.z = Target.z + Radius * cosf(Pitch) * cosf(Yaw);
    return pos;
}

glm::mat4 Camera::GetViewMatrix() const {
    // 【架构优化】：直接复用刚才写的 GetPosition 函数，代码瞬间清爽！
    glm::vec3 position = GetPosition();

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