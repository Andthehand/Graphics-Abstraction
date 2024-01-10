#pragma once
#include <vector>

#include <vulkan/vulkan.h>
#include <filesystem>

#include <glm/glm.hpp>

#include "VkBuffer.h"
#include "VkFrameBuffer.h"

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

	std::unique_ptr<FrameBuffer> m_FrameBuffer;

	VkPipelineLayout m_PipelineLayout;
	VkPipeline m_GraphicsPipeline;

	std::unique_ptr<Buffer> VertexBuffer;

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
	static void CreatePipeline();

	static void CreateCommandPool();
	static void CreateCommandBuffer();
	static void RecordCommandBuffer(const VkCommandBuffer commandBuffer, const uint32_t imageIndex);

	static void CreateSyncObjects();

	//TODO: Move this to a shader class
	static VkShaderModule CreateShaderModule(const std::vector<char>& code);
	static std::vector<char> ReadFile(const std::filesystem::path& filename);
private:
	inline static RendererData s_Data;
};

