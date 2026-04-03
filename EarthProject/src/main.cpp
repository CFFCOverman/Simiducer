#include <Simiducer.h>
#include "EarthLayer.h"

int main() {
    // 1. 启动引擎心智
    auto app = new Simiducer::Application();

    // 2. 插入你的地球图层
    app->PushLayer(new EarthLayer());

    // 3. 引擎接管控制权
    app->Run();

    delete app;
    return 0;
}