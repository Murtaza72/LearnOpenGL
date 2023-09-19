#pragma once

#include "Test.h"
#include "Shader.h"

#include <string>

namespace test
{
	class TestTextures : public Test {
	public:
		TestTextures();
		~TestTextures();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int LoadTexture(const std::string& path);

	private:
		unsigned int m_VBO, m_IBO, m_VAO;
		Shader m_Shader;
		unsigned int m_TextureID;
		unsigned char* m_ImageBuffer;
	};
}