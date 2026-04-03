#include <Simiducer.h>

int main() {
    auto app = new Simiducer::Application();
    app->Run();
    delete app;
    return 0;
}