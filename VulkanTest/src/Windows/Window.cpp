#include "Window.h"

Window::Window(uint32_t width, uint32_t height, const char* name) {
	InitWindow(width, height, name);

	Device::Get().Init(m_Window);
}

Window::~Window() {
	Device::Get().Shutdown();

	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void Window::OnUpdate() {
	glfwPollEvents();
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(m_Window);
}

void Window::InitWindow(uint32_t width, uint32_t height, const char* name) {
	glfwInit();
#ifdef Vulkan
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif // Vulkan
	m_Window = glfwCreateWindow(width, height, name, nullptr, nullptr);
}