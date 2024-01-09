#pragma once
#ifdef Vulkan
#define GLFW_INCLUDE_VULKAN
#endif // Vulkan
#include <GLFW/glfw3.h>

#include "GraphicsAPI.h"

class Window {
public:
	Window(uint32_t width, uint32_t height, const char* name);
	~Window();

	void OnUpdate();

	bool ShouldClose();

	GLFWwindow* GetWindowHandle() { return m_Window; }
private:
	void InitWindow(uint32_t width, uint32_t height, const char* name);
private:
	GLFWwindow* m_Window;
};