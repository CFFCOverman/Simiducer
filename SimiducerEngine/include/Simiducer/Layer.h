#pragma once

namespace Simiducer {
    class Layer {
    public:
        virtual ~Layer() = default;

        // 当这个图层被装载到引擎时触发（用于加载贴图、模型）
        virtual void OnAttach() {}

        // 当图层被卸载时触发（用于清理内存）
        virtual void OnDetach() {}

        // 引擎每帧都会调用这个函数（用于渲染 3D 地球）
        virtual void OnUpdate() {}

        // 引擎每帧都会调用这个函数（用于渲染 ImGui 时间轴面板）
        virtual void OnUIRender() {}

        // 【新增】：引擎有任何风吹草动（按键、缩放），都会调用这个函数通知图层
        virtual void OnEvent() {}
    };
}