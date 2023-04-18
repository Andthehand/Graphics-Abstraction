#pragma once
#include "Window.h"

#include <stdint.h>

class Application {
public:
    Application();
    ~Application() = default;

    void MainLoop();

    Window& GetWindow() { return m_Window; }

    static Application& Get() { return *s_Instance; }
private:
    static Application* s_Instance;

    Window m_Window;
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
};
