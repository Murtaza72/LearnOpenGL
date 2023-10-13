#pragma once

#include "tests/Test.h"

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
		unsigned int m_DepthMapFBO, m_DepthMap;
		Shader m_DepthShader, m_Shader;
		glm::vec3 m_LightPos;
		unsigned int m_WoodTexture;
	};
}