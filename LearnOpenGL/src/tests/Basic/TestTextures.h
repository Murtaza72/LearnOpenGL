#pragma once

#include "tests/Test.h"

namespace test {

	class TestTextures : public Test
	{
	public:
		TestTextures();
		~TestTextures();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VBO, m_IBO, m_VAO;
		Shader m_Shader;
		Texture m_Texture;
		unsigned char* m_ImageBuffer;
	};
}