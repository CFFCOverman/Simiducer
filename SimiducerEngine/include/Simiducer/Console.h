#pragma once
#include <string>
#include <vector>

namespace Simiducer {
    class Console {
    public:
        // 向控制台打印一条消息
        static void Log(const std::string& message);

        // 清空控制台
        static void Clear();

        // 渲染 UI 界面 (由 ImGui 驱动)
        static void Draw(const char* title, bool* p_open = nullptr);

    private:
        static std::vector<std::string> s_Messages; // 存放所有日志的容器
    };
}