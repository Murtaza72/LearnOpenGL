#pragma once

#include "Test.h"

#include "Shader.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

namespace test
{
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
		Camera m_Camera;
		glm::vec3 m_LightPos;
		float m_Shininess;
		unsigned int m_DiffuseMap, m_SpecularMap;
		int m_RotateCube;
	};
}