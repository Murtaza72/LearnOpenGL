#pragma once

#include "tests/Test.h"

namespace test {

	class TestInstancing : public Test
	{
	public:
		TestInstancing();
		~TestInstancing();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VAO, m_VBO, m_InstanceVBO;
		Shader m_Shader;
	};
}

