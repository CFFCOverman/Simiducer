#pragma once
#include <glad/glad.h>
#include <vector>

class Sphere {
public:
    // 构造函数：传入半径、经线数(切割多少瓣)、纬线数(切割多少层)
    Sphere(float radius, unsigned int sectorCount, unsigned int stackCount);

    void draw() const;    // 绘制球体
    void destroy();       // 清理显存

private:
    unsigned int VAO, VBO, EBO;
    unsigned int indexCount; // 记录总共有多少个顶点索引
};