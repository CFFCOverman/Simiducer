#pragma once

namespace Simiducer {
    // 抽象按键码 (与 GLFW 对应，但对外隐藏 GLFW)
    enum class Key {
        Space = 32,
        Escape = 256,
        W = 87, A = 65, S = 83, D = 68,
        // ... 需要时可以继续添加
    };

    class Input {
    public:
        // 查询某个按键当前是否被按下
        static bool IsKeyPressed(Key keycode);
    };
}