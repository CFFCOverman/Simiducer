#include "Simiducer/Texture.h"
#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

namespace Simiducer {
    Texture::Texture(const std::string& path)
        : m_RendererID(0), m_Width(0), m_Height(0), m_BPP(0)
    {
        // 翻转 Y 轴，因为 OpenGL 的纹理坐标原点在左下角，而图片通常在左上角
        stbi_set_flip_vertically_on_load(1);

        // 加载图片数据
        unsigned char* localBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

        if (localBuffer) {
            glGenTextures(1, &m_RendererID);
            glBindTexture(GL_TEXTURE_2D, m_RendererID);

            // 设置纹理环绕和过滤参数
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // 将数据传给 GPU 并生成 Mipmap
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer);
            glGenerateMipmap(GL_TEXTURE_2D);

            // 释放 CPU 内存
            stbi_image_free(localBuffer);
        }
        else {
            std::cerr << "Engine Error: Failed to load texture: " << path << std::endl;
        }
    }

    Texture::~Texture() {
        glDeleteTextures(1, &m_RendererID);
    }

    void Texture::Bind(unsigned int slot) const {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_RendererID);
    }

    void Texture::Unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}