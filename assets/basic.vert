#version 330 core

// 接收来自 Sphere.cpp 的顶点数据 (位置、法线、贴图坐标)
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

// 传给片段着色器 basic.frag 的变量 (注意这里名字必须叫 TexCoord)
out vec2 TexCoord;

// 接收来自 C++ 的矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // 计算 3D 顶点在屏幕上的最终位置
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    
    // 把贴图坐标原封不动地传给 basic.frag
    TexCoord = aTexCoords;
}