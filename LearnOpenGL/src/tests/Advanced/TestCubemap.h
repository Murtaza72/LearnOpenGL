#pragma once

#include "tests/Test.h"

namespace test {

	class TestCubemap : public Test
	{
	public:
		TestCubemap();
		~TestCubemap();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		std::vector<std::string> m_Faces;
		unsigned int m_CubemapTex;
		Shader m_Shader, m_SkyboxShader;
		Model m_Model;
		unsigned int m_SkyboxVAO, m_SkyboxVBO;
		Camera m_Camera;
	};
}