#pragma once
#include <vector>

#include <vulkan/vulkan.h>
#include <filesystem>

#include <glm/glm.hpp>

#include "VkBuffer.h"
#include "VkFrameBuffer.h"
#include "VkPipeline.h"
#include "VkShader.h"

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

	std::unique_ptr<Pipeline> m_Pipeline;

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
	static void CreateCommandPool();
	static void CreateCommandBuffer();
	static void RecordCommandBuffer(const VkCommandBuffer commandBuffer, const uint32_t imageIndex);

	static void CreateSyncObjects();

	static void RecreateSwapchain();
private:
	inline static RendererData s_Data;
};

