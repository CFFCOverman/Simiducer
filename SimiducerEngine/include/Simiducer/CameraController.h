#pragma once
#include "Simiducer/Camera.h"

namespace Simiducer {
    class CameraController {
    public:
        CameraController(Camera* camera);

        // 每帧检测鼠标是否被拖拽
        void OnUpdate(float dt);

        // 接收滚轮事件
        void Zoom(float yoffset);

    private:
        Camera* m_Camera;
    };
}