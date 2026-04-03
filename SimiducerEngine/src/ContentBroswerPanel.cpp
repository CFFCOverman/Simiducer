#include "Simiducer/ContentBrowserPanel.h"
#include <imgui.h>

namespace Simiducer {

    ContentBrowserPanel::ContentBrowserPanel() {
        // 为了确保你能直接看到效果，我们先硬编码到你的 assets 绝对路径
        // 以后我们可以把它改成相对于项目根目录的动态路径
        m_BaseDirectory = "C:/Users/pinyo/source/repos/Simiducer/assets";
        m_CurrentDirectory = m_BaseDirectory;
    }

    void ContentBrowserPanel::OnUIRender() {
        ImGui::Begin("Content Browser");

        // 如果当前目录不是根目录，就画一个“返回上一级”的按钮
        if (m_CurrentDirectory != m_BaseDirectory) {
            if (ImGui::Button("<- Back")) {
                m_CurrentDirectory = m_CurrentDirectory.parent_path();
            }
            ImGui::Separator();
        }

        // 遍历当前目录下的所有文件和文件夹
        for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory)) {
            const auto& path = directoryEntry.path();
            std::string filenameString = path.filename().string();

            if (directoryEntry.is_directory()) {
                // 如果是文件夹，画一个按钮，点击后进入该文件夹
                if (ImGui::Button(("[Dir]  " + filenameString).c_str())) {
                    m_CurrentDirectory /= path.filename();
                }
            }
            else {
                // 如果是文件，就只显示一段文字
                ImGui::Text("[File] %s", filenameString.c_str());
            }
        }

        ImGui::End();
    }
}