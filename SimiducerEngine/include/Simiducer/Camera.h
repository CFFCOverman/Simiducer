#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    // 摄像机状态参数
    float Radius;     // 轨道半径
    float Yaw;        // 偏航角 (左右)
    float Pitch;      // 俯仰角 (上下)
    glm::vec3 Target; // 观察目标 (我们的地球在中心，即 0,0,0)

    // 构造函数：设定初始距离和角度
    Camera(float radius = 3.5f, float yaw = 0.0f, float pitch = 0.0f);

    // 获取计算好的 View 观察矩阵
    glm::mat4 GetViewMatrix() const;

    // 处理鼠标拖拽输入
    void ProcessMouseMovement(float xoffset, float yoffset, float sensitivity = 0.005f);

    // 处理鼠标滚轮输入
    void ProcessMouseScroll(float yoffset);

    // 【新增】：获取当前摄像机的 3D 绝对坐标
    glm::vec3 GetPosition() const;
};