#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;    // 法线数据 (球体朝外的方向)
layout (location = 2) in vec2 aTexCoords; // 贴图 UV 坐标

out vec3 FragPos;   // 传给片段着色器的：当前像素在世界中的位置
out vec3 Normal;    // 传给片段着色器的：法线方向
out vec2 TexCoords; // 传给片段着色器的：贴图坐标

uniform mat4 model;      // 模型矩阵 (控制地球的位置/缩放/旋转)
uniform mat4 view;       // 观察矩阵 (摄像机)
uniform mat4 projection; // 投影矩阵 (透视)

void main() {
    // 计算当前顶点在世界坐标系中的真实位置
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // 计算正确的法线方向 (使用法线矩阵防止非均匀缩放导致光照错误)
    Normal = mat3(transpose(inverse(model))) * aNormal;
    
    TexCoords = aTexCoords;
    
    // 最终在屏幕上的位置
    gl_Position = projection * view * vec4(FragPos, 1.0);
}