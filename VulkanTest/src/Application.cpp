#include "Application.h"
Application::Application() : m_Window(800, 600, "Vulkan Test") { }

void Application::MainLoop() {
    while (!m_Window.ShouldClose()) {
        m_Window.OnUpdate();
    }
}
