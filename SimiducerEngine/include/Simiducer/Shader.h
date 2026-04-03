#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <fstream>   // 新增：文件流
#include <sstream>   // 新增：字符串流
#include <iostream>

class Shader {
public:
    unsigned int ID;

    // 构造函数现在接收的是【文件路径】，而不是直接的字符串代码了
    Shader(const char* vertexPath, const char* fragmentPath);

    void use();
    void destroy();
    void setMat4(const std::string& name, const glm::mat4& mat) const;
    // 传给 Shader 一个整数（常用来绑定纹理插槽）
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    // 传 glm::vec3
    void setVec3(const std::string& name, const glm::vec3& value) const;
    // 顺便加一个传三个 float 的重载，以后肯定用得上
    void setVec3(const std::string& name, float x, float y, float z) const;
};

