#pragma once
#include <functional>
#include <string>

namespace Simiducer {

    // 引擎支持的所有事件类型
    enum class EventType {
        None = 0,
        WindowClose, WindowResize,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    // 事件基类
    class Event {
    public:
        virtual ~Event() = default;

        bool Handled = false; // 如果事件被某个 Layer 处理了，就设为 true，阻止它继续向后传播

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual std::string ToString() const { return GetName(); }
    };

    // 事件分发器（核心大招：负责把包裹准确投递给对应的函数）
    class EventDispatcher {
    public:
        EventDispatcher(Event& event) : m_Event(event) {}

        // 这是一个模板函数，它会检查当前事件是不是我们要拦截的类型 T。
        // 如果是，就把事件转换成 T，并交给函数 F 去处理。
        template<typename T, typename F>
        bool Dispatch(const F& func) {
            if (m_Event.GetEventType() == T::GetStaticType()) {
                m_Event.Handled |= func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };
}