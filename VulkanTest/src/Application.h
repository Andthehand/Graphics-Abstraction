#pragma once
#include "Windows/Window.h"

#include <stdint.h>

class Application {
public:
    Application();
    ~Application();

    void MainLoop();

    Window& GetWindow() { return m_Window; }

    static Application& Get() { return *m_Instance; }
private:
    static Application* m_Instance;

    Window m_Window;
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
};
