#include "Application.h"

#include "Renderer/RenderCommand.h"

Application* Application::m_Instance = nullptr;

Application::Application() 
    : m_Window(800, 600, "Vulkan Test"){
    m_Instance = this;

    Renderer::Init();
    RenderCommand::Init();
    RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
}

Application::~Application() {
    Renderer::Shutdown();
}

void Application::MainLoop() {
    while (!m_Window.ShouldClose()) {
        m_Window.OnUpdate();
        Renderer::DrawFrame();

        m_Window.OnRender();
    }
}
