#include "Simiducer/Sphere.h"
#include <cmath>

const float PI = 3.14159265359f;

Sphere::Sphere(float radius, unsigned int sectorCount, unsigned int stackCount) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    float sectorStep = 2 * PI / sectorCount;
    float stackStep = PI / stackCount;
    float sectorAngle, stackAngle;

    // 1. 生成所有顶点的 3D 坐标
    for (unsigned int i = 0; i <= stackCount; ++i) {
        stackAngle = PI / 2 - i * stackStep; // 从北极(pi/2) 到 南极(-pi/2)
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep; // 从 0 到 2pi
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);

            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
    }

    // 2. 生成缝合三角形的索引 (Indices)
    for (unsigned int i = 0; i < stackCount; ++i) {
        unsigned int k1 = i * (sectorCount + 1);     // 当前纬线圈的起点
        unsigned int k2 = k1 + sectorCount + 1;      // 下一条纬线圈的起点

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    indexCount = indices.size();

    // 3. 将数据送入 GPU
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // 告诉显卡怎么读取顶点 (现在只有 x, y, z，所以跨度是 3 * sizeof(float))
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // 解绑 VAO 保护数据
}

void Sphere::draw() const {
    glBindVertexArray(VAO);
    // 使用 glDrawElements 而不是 glDrawArrays，因为我们使用了索引(EBO)来复用顶点
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Sphere::destroy() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

