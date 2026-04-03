#pragma once
#include <string>

namespace Simiducer {
    class Texture {
    public:
        // 传入图片路径即可自动加载
        Texture(const std::string& path);
        ~Texture();

        // 绑定贴图到渲染管线（默认插槽 0）
        void Bind(unsigned int slot = 0) const;
        void Unbind() const;

    private:
        unsigned int m_RendererID;
        int m_Width, m_Height, m_BPP;
    };
}