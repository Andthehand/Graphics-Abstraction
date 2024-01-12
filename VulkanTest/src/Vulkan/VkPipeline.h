#pragma once
#include <vulkan/vulkan.h>

#include <vector>
#include <filesystem>

#include "VkBuffer.h"
#include "VkShader.h"
#include "VkFramebuffer.h"

enum class DynamicStates {
	Viewport,
	Scissor,
	LineWidth
};

struct PipelineDescription {
	std::shared_ptr<Framebuffer> Framebuffer;

	std::shared_ptr<Shader> Shaders;

	std::shared_ptr<Buffer> VertexBuffer;

	std::vector<DynamicStates> DynamicStates;

	std::vector<VkDynamicState> GetVkDynamicStates() {
		std::vector<VkDynamicState> dynamicStates;
		for (auto state : DynamicStates) {
			switch (state) {
				case DynamicStates::Viewport:
					dynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
					break;
				case DynamicStates::Scissor:
					dynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);
					break;
				case DynamicStates::LineWidth:
					dynamicStates.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);
					break;
			}
		}
		return dynamicStates;
	}
};

class Pipeline {
public:
	Pipeline(PipelineDescription pipelineDescription);
	~Pipeline();

	void Bind(const VkCommandBuffer commandBuffer);

	void BeginRenderPass(const VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void RecreateSwapchain();

	inline VkSwapchainKHR GetSwapchain() const { return m_PipelineDescription.Framebuffer->GetSwapchain(); }
	inline VkExtent2D GetExtent() const { return m_PipelineDescription.Framebuffer->GetExtent(); }
	inline VkRenderPass GetRenderPass() const { return m_PipelineDescription.Framebuffer->GetRenderPass(); }
	inline VkFramebuffer GetFramebuffer(uint32_t index) const { return m_PipelineDescription.Framebuffer->GetFramebuffer(index); }
private:
	void CreatePipeline();
private:
	PipelineDescription m_PipelineDescription;
	VkPipeline m_Pipeline;
	VkPipelineLayout m_PipelineLayout;
};

