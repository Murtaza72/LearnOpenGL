#pragma once

#include "Test.h"

#include "glm/glm.hpp"

#include "Shader.h"
#include "Camera.h"

namespace test
{
	class TestPhongLighting : public Test
	{
	public:
		TestPhongLighting();
		~TestPhongLighting();

		void OnUpdate() override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VBO, m_CubeVAO, m_LightCubeVAO;
		glm::vec3 m_LightPos;
		Shader m_LightingShader;
		Shader m_LightCubeShader;

		Camera m_Camera;

		int SCREEN_WIDTH = 800;
		int SCREEN_HEIGHT = 600;
	};
}