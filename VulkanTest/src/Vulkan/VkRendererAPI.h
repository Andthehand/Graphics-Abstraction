#pragma once
#include <cstdint>

#include <glm/glm.hpp>

class RendererAPI {
public:
	void Init();

	void SetClearColor(const glm::vec4& color) { m_ClearColor = color; }
	const glm::vec4& GetClearColor() const { return m_ClearColor; }
private:
	glm::vec4 m_ClearColor;
};

