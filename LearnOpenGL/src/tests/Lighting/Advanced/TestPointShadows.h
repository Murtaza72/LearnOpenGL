#pragma once

#include "tests/Test.h"

namespace test {

	class TestPointShadows : public Test
	{
	public:
		TestPointShadows();
		~TestPointShadows();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		int SHADOW_WIDTH_POINT;
		int SHADOW_HEIGHT_POINT;

		unsigned int m_DepthMapFBO, m_DepthCubemap;
		glm::vec3 m_LightPos;
		Shader m_DepthShader;
		Shader m_Shader;
		unsigned int m_WoodTexture;
	};
}
