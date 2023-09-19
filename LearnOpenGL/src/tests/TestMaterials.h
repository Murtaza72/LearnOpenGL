#pragma once

#include "Test.h"

#include "Shader.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

namespace test {

	class TestMaterials :public Test
	{
	public:
		TestMaterials();
		~TestMaterials();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;


	private:
		unsigned int m_VBO, m_LightCubeVAO, m_CubeVAO;
		Shader m_LightingShader, m_LightCubeShader;
		Camera m_Camera;
		glm::vec3 m_ObjectColor, m_LightColor, m_LightPos;
		glm::vec3 m_AmbientColor, m_DiffuseColor;
		float m_Shininess;
	};
}