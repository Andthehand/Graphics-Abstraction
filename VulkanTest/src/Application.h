#pragma once
#include "Window.h"

class Application {
public:
    Application();
    ~Application() = default;

    void MainLoop();

private:
    Window m_Window;
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
};
