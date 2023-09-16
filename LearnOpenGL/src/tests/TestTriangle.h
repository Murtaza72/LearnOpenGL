#pragma once

#include "Test.h"

#include "Shader.h"

namespace test
{
	class TestTriangle :public Test
	{
	public:
		TestTriangle();
		~TestTriangle();

		void OnRender() override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VBO, m_VAO;

		Shader m_Shader;
	};
}