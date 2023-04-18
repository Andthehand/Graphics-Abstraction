#include "Application.h"

Application* Application::s_Instance = nullptr;

Application::Application() {
    s_Instance = this;
    m_Window.Init(800, 600, "Vulkan Test");
}

void Application::MainLoop() {
    while (!m_Window.ShouldClose()) {
        m_Window.OnUpdate();
    }
}
