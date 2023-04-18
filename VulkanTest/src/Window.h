#pragma once
#include "Vulkan/VBO.h"

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

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR Capabilities;
	std::vector<VkSurfaceFormatKHR> Formats;
	std::vector<VkPresentModeKHR> PresentModes;
};

class Window {
public:
	Window() = default;
	~Window();

	void Init(uint32_t width, uint32_t height, const char* name);

	bool ShouldClose();

	void OnUpdate();

	VkPhysicalDevice GetPhysicalDevice() { return m_PhysicalDevice; }
	VkDevice GetDevice() { return m_Device; }
private:
	void DrawFrame();

	void InitVulkan();
	void CreateInstance();
	void SetupDebugMessenger();
	void CreateSurface();
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	bool IsDeviceSuitable(VkPhysicalDevice device);

	void InitWindow(uint32_t width, uint32_t height, const char* name);

	static void FramebufferResizeCallback(GLFWwindow* window, int width, int height);

	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();

	void RecreateSwapChain();

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateSwapChain();
	void CreateImageViews();
	void CreateRenderPass();

	void CleanupSwapChain();

	VkShaderModule CreateShaderModule(const std::vector<char>& code);
	void CreateGraphicsPipeline();

	void CreateFramebuffers();

	void CreateCommandPool();
	void CreateCommandBuffer();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void CreateSyncObjects();
private:
#ifdef DEBUG
	const bool m_EnableValidationLayers = true;
#else
	const bool m_EnableValidationLayers = false;
#endif

	VkInstance m_Instance;

	const std::vector<const char*> m_ValidationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};
	VkDebugUtilsMessengerEXT m_DebugMessenger;
	
	GLFWwindow* m_Window;
	VkSurfaceKHR m_Surface;

	const std::vector<const char*> m_DeviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
	VkDevice m_Device;

	VkSwapchainKHR m_SwapChain;
	std::vector<VkImage> m_SwapChainImages;
	VkFormat m_SwapChainImageFormat;
	VkExtent2D m_SwapChainExtent;
	std::vector<VkImageView> m_SwapChainImageViews;

	VkRenderPass m_RenderPass;
	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;

	std::vector<VkFramebuffer> m_SwapChainFramebuffers;

	VkCommandPool m_CommandPool;
	std::vector<VkCommandBuffer> m_CommandBuffers;

	VkQueue m_GraphicsQueue, m_PresentQueue;

	const int MAX_FRAMES_IN_FLIGHT = 2;
	std::vector<VkSemaphore> m_ImageAvailableSemaphores;
	std::vector<VkSemaphore> m_RenderFinishedSemaphores;
	std::vector<VkFence> m_InFlightFences;

	VBO m_VertexBuffer;

	bool m_FramebufferResized = false;

	uint32_t m_CurrentFrame = 0;
};