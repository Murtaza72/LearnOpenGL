#pragma once

#include "tests/Test.h"

namespace test {

	class TestTriangle : public Test
	{
	public:
		TestTriangle();
		~TestTriangle();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VBO, m_VAO;

		Shader m_Shader;
	};
}