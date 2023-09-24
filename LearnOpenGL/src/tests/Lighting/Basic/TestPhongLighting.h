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
		glm::vec3 m_LightPos;
		Shader m_LightingShader;
		Shader m_LightCubeShader;
		Camera m_Camera;
		glm::vec3 m_ObjectColor;
		glm::vec3 m_LightColor;
		int m_RotateCube;
	};
}