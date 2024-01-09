#pragma once
#include <vector>

#include <vulkan/vulkan.h>
#include <filesystem>

#include <glm/glm.hpp>

#include "VkBuffer.h"

struct QuadVertex {
	glm::vec2 pos;
	glm::vec3 color;
};

struct RendererData {
	const std::vector<QuadVertex> vertices = {
		{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
		{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	static const uint32_t MaxQuads = 1000;
	static const uint32_t MaxVertices = MaxQuads * 4;
	static const uint32_t MaxIndices = MaxQuads * 6;

	VkSwapchainKHR m_SwapChain;
	std::vector<VkImage> m_SwapChainImages;
	VkFormat m_SwapChainImageFormat;
	VkExtent2D m_SwapChainExtent;
	std::vector<VkImageView> m_SwapChainImageViews;

	VkRenderPass m_RenderPass;

	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;

	std::unique_ptr<Buffer> VertexBuffer;

	std::vector<VkFramebuffer> m_SwapChainFramebuffers;

	VkCommandPool m_CommandPool;
	std::vector<VkCommandBuffer> m_CommandBuffers;

	std::vector<VkSemaphore> m_ImageAvailableSemaphores;
	std::vector<VkSemaphore> m_RenderFinishedSemaphores;
	std::vector<VkFence> m_InFlightFences;
};

class Renderer {
public:
	static void Init();
	static void Shutdown();

	static void DrawFrame();

private:
	static void CreateSwapChain();
	static void CreateImageViews();
	static void CreateRenderPass();
	static void CreatePipeline();
	static void CreateFramebuffers();

	static void CreateCommandPool();
	static void CreateCommandBuffer();
	static void RecordCommandBuffer(const VkCommandBuffer commandBuffer, const uint32_t imageIndex);

	static void CreateSyncObjects();

	static void CleanupSwapChain();

	static VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	static VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	static VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	//TODO: Move this to a shader class
	static VkShaderModule CreateShaderModule(const std::vector<char>& code);
	static std::vector<char> ReadFile(const std::filesystem::path& filename);
private:
	inline static RendererData s_Data;
};

