#pragma once
#include <cstdint>
#include <memory>

#include <glm/glm.hpp>

#include "VkFramebuffer.h"

class RendererAPI {
public:
	void Init();

	void SetClearColor(const glm::vec4& color) { m_ClearColor = color; }
	const glm::vec4& GetClearColor() const { return m_ClearColor; }

	void Draw(const std::shared_ptr<Framebuffer> frameBuffer, uint32_t count);
	void DrawIndexed(const std::shared_ptr<Framebuffer> frameBuffer, uint32_t indexCount);
private:
	glm::vec4 m_ClearColor;
};

