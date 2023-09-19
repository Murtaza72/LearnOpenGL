#pragma once

#include "Test.h"

#include "glm/glm.hpp"

#include "Shader.h"
#include "Camera.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

namespace test
{
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
	};
}