#pragma once
#include <filesystem>
#include <vector>

#include <vulkan/vulkan.h>

class Shader {
public:
	Shader(std::filesystem::path& vertexShader, std::filesystem::path& fragmentShader);
	~Shader();

	std::vector<VkPipelineShaderStageCreateInfo> GetShaderStages() const;
private:
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
	std::vector<char> ReadFile(const std::filesystem::path& filename);
private:
	VkShaderModule m_VertexShaderModule;
	VkShaderModule m_FragmentShaderModule;
};
