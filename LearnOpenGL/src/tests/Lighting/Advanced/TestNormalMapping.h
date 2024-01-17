#pragma once

#include "tests/Test.h"

namespace test {

	class TestNormalMapping : public Test
	{
	public:
		TestNormalMapping();
		~TestNormalMapping();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VAO, m_VBO;
		Texture m_DiffuseTex, m_NormalTex;
		Shader m_Shader;
		glm::vec3 m_LightPos;
	};
}
