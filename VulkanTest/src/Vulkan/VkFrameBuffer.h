#pragma once
#include <initializer_list>
#include <vector>
#include <stdexcept>

#include <vulkan/vulkan.h>

enum class FramebufferTextureFormat {
	None = 0,

	// Color
	RGBA8,
	RED_INTEGER,

	// Depth/stencil
	DEPTH24STENCIL8,

	// Defaults
	Depth = DEPTH24STENCIL8
};

struct FramebufferTextureSpecification {
	FramebufferTextureSpecification() = default;
	FramebufferTextureSpecification(FramebufferTextureFormat format)
		: TextureFormat(format) {}

	FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
	// TODO: filtering/wrap

	VkImageLayout GetVkImageLayout() const {
		switch (TextureFormat) {
			case FramebufferTextureFormat::DEPTH24STENCIL8: 
				return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			case FramebufferTextureFormat::RGBA8:
			case FramebufferTextureFormat::RED_INTEGER: 
				return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			default: 
				std::runtime_error("Unsupported texture format!");
				break;
		}
		return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}
};

struct FramebufferAttachmentSpecification {
	FramebufferAttachmentSpecification() = default;
	FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
		: Attachments(attachments) {}

	std::vector<FramebufferTextureSpecification> Attachments;
};

struct FramebufferDescription {
	FramebufferAttachmentSpecification Attachments;

	uint32_t Width, Height;
	uint32_t Samples = 1;

	VkSampleCountFlagBits GetVkSampleCount() const {
		switch (Samples) {
			case 1: return VK_SAMPLE_COUNT_1_BIT;
			case 2: return VK_SAMPLE_COUNT_2_BIT;
			case 4: return VK_SAMPLE_COUNT_4_BIT;
			case 8: return VK_SAMPLE_COUNT_8_BIT;
			case 16: return VK_SAMPLE_COUNT_16_BIT;
			case 32: return VK_SAMPLE_COUNT_32_BIT;
			case 64: return VK_SAMPLE_COUNT_64_BIT;
			default: 
				std::runtime_error("Unsupported sample count!");
				break;
		}

		return VK_SAMPLE_COUNT_1_BIT;
	}
};

class FrameBuffer {
public:
	FrameBuffer(const FramebufferDescription& frameBufferSpecification);
	~FrameBuffer();

	VkSwapchainKHR GetSwapChain() const { return m_SwapChain; }
	VkRenderPass GetRenderPass() const { return m_RenderPass; }

	VkFramebuffer GetFramebuffer(uint32_t index) const { return m_Framebuffers[index]; }

	const VkExtent2D& GetExtent() const { return m_SwapChainExtent; }
private:
	void CreateSwapChain();
	void CreateImageViews();

	void CreateRenderPass();

	void CreateFramebuffers();

	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
private:
	FramebufferDescription m_FramebufferDescription;

	VkSwapchainKHR m_SwapChain;
	std::vector<VkImage> m_SwapChainImages;
	VkFormat m_SwapChainImageFormat;
	VkExtent2D m_SwapChainExtent;
	std::vector<VkImageView> m_SwapChainImageViews;
	
	VkRenderPass m_RenderPass;

	std::vector<VkFramebuffer> m_Framebuffers;
};

