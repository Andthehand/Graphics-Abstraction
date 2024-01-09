#pragma once
#include <vulkan/vulkan.h>

#include <vector>
#include <filesystem>

#include "VkBuffer.h"

enum class DynamicStates {
	Viewport,
	Scissor,
	LineWidth
};

struct PipelineDescription {
	//TODO: Add Vertex and fragment shaders

	Buffer VertexBuffer;

	std::vector<DynamicStates> DynamicStates;

	VkExtent2D ViewportExtent;
	VkFormat ColorFormat;

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
private:
	void CreateRenderPass();
	void CreatePipeline();

	//TODO: Move this to a shader class
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
	std::vector<char> ReadFile(const std::filesystem::path& filename);
private:
	VkRenderPass m_RenderPass;

	PipelineDescription m_PipelineDescription;
	VkPipeline m_Pipeline;
	VkPipelineLayout m_PipelineLayout;
};

