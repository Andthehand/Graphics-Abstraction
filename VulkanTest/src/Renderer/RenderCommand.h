#pragma once
#include <memory>

#include <glm/glm.hpp>

#include "GraphicsAPI.h"

class RenderCommand {
public:
	inline static void Init() {
		s_RendererAPI = std::make_unique<RendererAPI>();
		s_RendererAPI->Init();
	}

	inline static void SetClearColor(const glm::vec4& color) {
		s_RendererAPI->SetClearColor(color);
	}

	inline static RendererAPI* GetRendererAPI() { return s_RendererAPI.get(); }
private:
	inline static std::unique_ptr<RendererAPI> s_RendererAPI;
};

