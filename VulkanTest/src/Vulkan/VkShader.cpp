#include "VkShader.h"

#include "VkDevice.h"

#include <fstream>

Shader::Shader(std::filesystem::path& vertexShader, std::filesystem::path& fragmentShader) {
    auto vertShaderCode = ReadFile(vertexShader);
    auto fragShaderCode = ReadFile(fragmentShader);

    m_VertexShaderModule = CreateShaderModule(vertShaderCode);
    m_FragmentShaderModule = CreateShaderModule(fragShaderCode);
}

Shader::~Shader() {
    VkDevice device = Device::Get().GetDevice();

	vkDestroyShaderModule(device, m_VertexShaderModule, nullptr);
	vkDestroyShaderModule(device, m_FragmentShaderModule, nullptr);
}

std::vector<VkPipelineShaderStageCreateInfo> Shader::GetShaderStages() const {
    VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
    vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module = m_VertexShaderModule;
    vertexShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
    fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = m_FragmentShaderModule;
    fragmentShaderStageInfo.pName = "main";

    return { vertexShaderStageInfo, fragmentShaderStageInfo };
}

VkShaderModule Shader::CreateShaderModule(const std::vector<char>& code) {
    VkDevice device = Device::Get().GetDevice();

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

std::vector<char> Shader::ReadFile(const std::filesystem::path& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}
