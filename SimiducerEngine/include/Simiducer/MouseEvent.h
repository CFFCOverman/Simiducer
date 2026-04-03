#pragma once
#include "Simiducer/Event.h"

namespace Simiducer {

    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {
        }

        float GetXOffset() const { return m_XOffset; }
        float GetYOffset() const { return m_YOffset; }

        // 静态类型，用于 Dispatcher 进行类型比对
        static EventType GetStaticType() { return EventType::MouseScrolled; }

        virtual EventType GetEventType() const override { return GetStaticType(); }
        virtual const char* GetName() const override { return "MouseScrolledEvent"; }

    private:
        float m_XOffset, m_YOffset;
    };

}