#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <vector>
#include <optional>

struct QueueFamilyIndices {
	std::optional<uint32_t> GraphicsFamily;
	std::optional<uint32_t> PresentFamily;

	bool IsComplete() {
		return GraphicsFamily.has_value() && PresentFamily.has_value();
	}
};

class Window {
public:
	Window(uint32_t width, uint32_t height, const char* name);
	~Window();

	bool ShouldClose();

	void OnUpdate();

private:
	void InitVulkan();
	void CreateInstance();
	void SetupDebugMessenger();
	void CreateSurface();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	bool IsDeviceSuitable(VkPhysicalDevice device);

	void InitWindow(uint32_t width, uint32_t height, const char* name);

	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();
private:
	GLFWwindow* m_Window;

	const std::vector<const char*> m_ValidationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};
	VkInstance m_Instance;
	VkDebugUtilsMessengerEXT m_DebugMessenger;
	VkSurfaceKHR m_Surface;
	VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
	VkDevice m_Device;

	VkQueue m_GraphicsQueue, m_PresentQueue;

#ifdef DEBUG
	const bool m_EnableValidationLayers = true;
#else
	const bool m_EnableValidationLayers = false;
#endif
};