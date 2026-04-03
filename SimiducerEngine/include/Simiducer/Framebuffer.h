#pragma once
#include <cstdint>

namespace Simiducer {
    class Framebuffer {
    public:
        Framebuffer(uint32_t width, uint32_t height);
        ~Framebuffer();

        // 告诉显卡：接下来的东西画到我这里！
        void Bind();
        // 告诉显卡：画完了，切回默认屏幕！
        void Unbind();
        // 如果改变了窗口大小，需要重新生成画布
        void Resize(uint32_t width, uint32_t height);

        // 拿到画好的“图片”的 ID，等下给 ImGui 用
        uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }

    private:
        void Invalidate(); // 内部核心：真正创建/重建 FBO 的函数

        uint32_t m_RendererID = 0;
        uint32_t m_ColorAttachment = 0; // 存颜色的贴图
        uint32_t m_DepthAttachment = 0; // 存深度的贴图 (防止背面透视)
        uint32_t m_Width = 0, m_Height = 0;
    };
}