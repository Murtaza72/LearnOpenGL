#pragma once

#include "tests/Test.h"

namespace test {

	class TestParallaxMapping : public Test
	{
	public:
		TestParallaxMapping();
		~TestParallaxMapping();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VAO;
		Texture m_DiffuseTex, m_NormalTex, m_ParallaxTex;
		Shader m_Shader;
		glm::vec3 m_LightPos;
		float m_HeightScale;
	};
}
