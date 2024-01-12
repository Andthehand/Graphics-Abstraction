#include "VkRenderer.h"
#include <stdexcept>
#include <algorithm>
#include <fstream>

#include "VkDevice.h"
#include "VkRendererAPI.h"

#include "Application.h"
#include "Renderer/RenderCommand.h"

void Renderer::Init() {
    //Vertex Buffer Init
    BufferDescription bufferDescription{};
    bufferDescription.Type = BufferType::VertexBuffer;
    bufferDescription.Size = s_Data.MaxVertices * sizeof(QuadVertex);
    bufferDescription.Layout = {
        { ShaderDataType::Float2, "a_Position" },
		{ ShaderDataType::Float3, "a_Color" }
	};

    std::shared_ptr<Buffer> vertexBuffer = std::make_shared<Buffer>(bufferDescription);
    vertexBuffer->SetData(s_Data.vertices.data(), s_Data.vertices.size() * sizeof(QuadVertex));

    //Framebuffer Init
    FramebufferDescription framebufferDescriptions{};
    framebufferDescriptions.Attachments = { {FramebufferTextureFormat::RGBA8} };
    framebufferDescriptions.Width = 1280;
    framebufferDescriptions.Height = 720;
    std::shared_ptr<Framebuffer> framebuffer = std::make_shared<Framebuffer>(framebufferDescriptions);

    //Pipeline Init
    PipelineDescription pipelineDescription{};
    pipelineDescription.Framebuffer = framebuffer;
    pipelineDescription.Shaders = std::make_shared<Shader>((std::filesystem::path)"shaders/vert.spv", (std::filesystem::path)"shaders/frag.spv");
    pipelineDescription.VertexBuffer = vertexBuffer;
    pipelineDescription.DynamicStates = { DynamicStates::Viewport, DynamicStates::Scissor };

    s_Data.m_Pipeline = std::make_unique<Pipeline>(pipelineDescription);

    CreateCommandPool();
    CreateCommandBuffer();

    CreateSyncObjects();
}

void Renderer::Shutdown() {
    VkDevice device = Device::Get().GetDevice();
    vkDeviceWaitIdle(device);

    vkDestroyCommandPool(device, s_Data.m_CommandPool, nullptr);

    for (size_t i = 0; i < Device::MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(device, s_Data.m_RenderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device, s_Data.m_ImageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device, s_Data.m_InFlightFences[i], nullptr);
    }

    s_Data.m_Pipeline.reset();
}

void Renderer::DrawFrame() {
    VkDevice device = Device::Get().GetDevice();
    const uint32_t currentFrame = Device::Get().GetCurrentFrame();

    vkWaitForFences(device, 1, &s_Data.m_InFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, s_Data.m_Pipeline->GetSwapchain(), UINT64_MAX, s_Data.m_ImageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        RecreateSwapchain();
        return;
    }

    vkResetFences(device, 1, &s_Data.m_InFlightFences[currentFrame]);

    vkResetCommandBuffer(s_Data.m_CommandBuffers[currentFrame], 0);
    RecordCommandBuffer(s_Data.m_CommandBuffers[currentFrame], imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { s_Data.m_ImageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &s_Data.m_CommandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = { s_Data.m_RenderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(Device::Get().GetGraphicsQueue(), 1, &submitInfo, s_Data.m_InFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { s_Data.m_Pipeline->GetSwapchain() };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(Device::Get().GetPresentQueue(), &presentInfo);
}

void Renderer::CreateCommandPool() {
    VkDevice device = Device::Get().GetDevice();
    QueueFamilyIndices queueFamilyIndices = Device::Get().GetQueueFamilyIndices();

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.GraphicsFamily.value();

    if (vkCreateCommandPool(device, &poolInfo, nullptr, &s_Data.m_CommandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void Renderer::CreateCommandBuffer() {
    VkDevice device = Device::Get().GetDevice();
    s_Data.m_CommandBuffers.resize(Device::MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = s_Data.m_CommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)s_Data.m_CommandBuffers.size();

    if (vkAllocateCommandBuffers(device, &allocInfo, s_Data.m_CommandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void Renderer::RecordCommandBuffer(const VkCommandBuffer commandBuffer, const uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkExtent2D extent = s_Data.m_Pipeline->GetExtent();

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = s_Data.m_Pipeline->GetRenderPass();
    renderPassInfo.framebuffer = s_Data.m_Pipeline->GetFramebuffer(imageIndex);
    renderPassInfo.renderArea.offset = { 0, 0 };
    renderPassInfo.renderArea.extent = extent;

    const glm::vec4& clearColor = RenderCommand::GetRendererAPI()->GetClearColor();
    VkClearValue clearColorValue = { {clearColor.r, clearColor.g, clearColor.b, clearColor.a} };
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColorValue;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    s_Data.m_Pipeline->Bind(commandBuffer);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)extent.width;
    viewport.height = (float)extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = { 0, 0 };
    scissor.extent = extent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdDraw(commandBuffer, static_cast<uint32_t>(s_Data.vertices.size()), 1, 0, 0);

    vkCmdEndRenderPass(commandBuffer);
    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void Renderer::CreateSyncObjects() {
    VkDevice device = Device::Get().GetDevice();

    s_Data.m_ImageAvailableSemaphores.resize(Device::MAX_FRAMES_IN_FLIGHT);
    s_Data.m_RenderFinishedSemaphores.resize(Device::MAX_FRAMES_IN_FLIGHT);
    s_Data.m_InFlightFences.resize(Device::MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < Device::MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &s_Data.m_ImageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &s_Data.m_RenderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &s_Data.m_InFlightFences[i]) != VK_SUCCESS) {

            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

void Renderer::RecreateSwapchain() {
    //This is in case the window is minimized it will wait until it is restored
    int width = 0, height = 0;
    glfwGetFramebufferSize(Application::Get().GetWindow().GetWindowHandle(), &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(Application::Get().GetWindow().GetWindowHandle(), &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(Device::Get().GetDevice());

    s_Data.m_Pipeline->RecreateSwapchain();
}
