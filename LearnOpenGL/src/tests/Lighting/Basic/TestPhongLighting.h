#pragma once

#include "tests/Test.h"

namespace test {

	class TestPhongLighting : public Test
	{
	public:
		TestPhongLighting();
		~TestPhongLighting();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VBO, m_CubeVAO, m_LightCubeVAO;
		Shader m_LightingShader, m_LightCubeShader;
		Camera m_Camera;

		glm::vec3 m_ObjectColor, m_LightColor;
		glm::vec3 m_LightPos;
		int m_RotateCube, m_Shininess;
	};
}