#pragma once

#include "tests/Test.h"

namespace test {

	class TestLightingMaps : public Test
	{
	public:
		TestLightingMaps();
		~TestLightingMaps();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VBO, m_LightCubeVAO, m_CubeVAO;
		Shader m_LightingShader, m_LightCubeShader;
		Texture m_DiffuseMap, m_SpecularMap, m_EmissionMap;
		Camera m_Camera;

		glm::vec3 m_LightPos;
		float m_Shininess;
		int m_RotateCube;
	};
}