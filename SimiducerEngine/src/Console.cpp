#include "Simiducer/Console.h"
#include <imgui.h>

namespace Simiducer {
    // 初始化静态变量
    std::vector<std::string> Console::s_Messages;

    void Console::Log(const std::string& message) {
        s_Messages.push_back(message);
        // 为了防止内存爆炸，如果日志超过 1000 条，删掉最老的 100 条
        if (s_Messages.size() > 1000) {
            s_Messages.erase(s_Messages.begin(), s_Messages.begin() + 100);
        }
    }

    void Console::Clear() {
        s_Messages.clear();
    }

    void Console::Draw(const char* title, bool* p_open) {
        // 设置默认大小
        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin(title, p_open)) {
            ImGui::End();
            return;
        }

        // 顶部按钮栏
        if (ImGui::Button("Clear")) Clear();
        ImGui::SameLine();
        bool copy_to_clipboard = ImGui::Button("Copy All");

        ImGui::Separator();

        // 创建一个带滚动条的子区域
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (copy_to_clipboard) ImGui::LogToClipboard();

        // 打印所有储存的信息
        for (const auto& msg : s_Messages) {
            ImGui::TextUnformatted(msg.c_str());
        }

        if (copy_to_clipboard) ImGui::LogFinish();

        // 智能自动滚动到底部 (如果有新消息的话)
        if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndChild();
        ImGui::End();
    }
}