#pragma once
#include <filesystem>

namespace Simiducer {
    class ContentBrowserPanel {
    public:
        ContentBrowserPanel();

        // 每一帧调用，用来画 UI
        void OnUIRender();

    private:
        // 记录项目的根资产目录
        std::filesystem::path m_BaseDirectory;
        // 记录当前双击进去的目录
        std::filesystem::path m_CurrentDirectory;
    };
}