#pragma once

#include "tests/Test.h"

#include <Framebuffer.h>

namespace test {

	class TestDeferredRendering : public Test
	{
	public:
		TestDeferredRendering();
		~TestDeferredRendering();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		Framebuffer m_GBuffer;
		Model m_BackpackModel;
		Shader m_GeometryPassShader, m_LightingPassShader, m_LightCubeShader;
		std::vector<glm::vec3> m_ModelPositions;
		std::pair<std::vector<glm::vec3>, std::vector<glm::vec3>> m_LightCubes; // Pos, Color
		glm::vec3 m_ClearColor;
	};
}
