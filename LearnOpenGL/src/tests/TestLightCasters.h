#pragma once

#include "Test.h"

namespace test
{
	class TestLightCasters : public Test {
	public:
		TestLightCasters();
		~TestLightCasters();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VBO, m_LightCubeVAO, m_CubeVAO;
		Shader m_LightingShader, m_LightCubeShader;
		Camera m_Camera;
		glm::vec3 m_LightPos;
		float m_Shininess;
		unsigned int m_DiffuseMap, m_SpecularMap;
		int m_RotateCube;
		std::vector<glm::vec3> m_CubePositions;
		float m_RotationSpeed, m_Radius;
	};
}