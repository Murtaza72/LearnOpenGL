#pragma once

#include "tests/Test.h"

namespace test {

	class TestAntiAliasing : public Test
	{
	public:
		TestAntiAliasing();
		~TestAntiAliasing();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VAO, m_VBO;
		Shader m_Shader;
	};

}
