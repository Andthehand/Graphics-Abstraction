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

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR Capabilities;
	std::vector<VkSurfaceFormatKHR> Formats;
	std::vector<VkPresentModeKHR> PresentModes;
};

class Window {
public:
	Window(uint32_t width, uint32_t height, const char* name);
	~Window();

	bool ShouldClose();

	void OnUpdate();

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

	bool CheckValidationLayerSupport();
	std::vector<const char*> GetRequiredExtensions();

	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateSwapChain();

	void CreateImageViews();

	void CreateRenderPass();

	VkShaderModule CreateShaderModule(const std::vector<char>& code);
	void CreateGraphicsPipeline();

	void CreateFramebuffers();

	void CreateCommandPool();
	void CreateCommandBuffer();
	void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void CreateSyncObjects();
private:
	VkInstance m_Instance;

	const std::vector<const char*> m_ValidationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};
	VkDebugUtilsMessengerEXT m_DebugMessenger;
	
	GLFWwindow* m_Window;
	VkSurfaceKHR m_Surface;

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
	VkCommandBuffer m_CommandBuffer;

	VkQueue m_GraphicsQueue, m_PresentQueue;

	VkSemaphore m_ImageAvailableSemaphore;
	VkSemaphore m_RenderFinishedSemaphore;
	VkFence m_InFlightFence;

	const std::vector<const char*> m_DeviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

#ifdef DEBUG
	const bool m_EnableValidationLayers = true;
#else
	const bool m_EnableValidationLayers = false;
#endif
};