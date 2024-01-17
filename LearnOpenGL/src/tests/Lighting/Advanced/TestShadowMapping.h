#pragma once

#include "tests/Test.h"

#include "Framebuffer.h"

const int SHADOW_WIDTH = SCREEN_WIDTH;
const int SHADOW_HEIGHT = SCREEN_HEIGHT;

namespace test {

	class TestShadowMapping : public Test
	{
	public:
		TestShadowMapping();
		~TestShadowMapping();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		Framebuffer m_DepthFBO;
		Shader m_DepthShader, m_Shader;
		glm::vec3 m_LightPos;
		Texture m_WoodTexture;
	};
}