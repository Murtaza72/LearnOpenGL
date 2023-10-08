#pragma once

#include "tests/Test.h"

namespace test {

	class TestFrameBuffers : public Test
	{
	public:
		TestFrameBuffers();
		~TestFrameBuffers();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_FrameBuffer, m_TextureColorBuffer, m_RenderBuffer;
		unsigned int m_QuadVAO, m_QuadVBO;
		Shader m_Shader, m_ScreenShader;
		Model m_Model;
		float m_Kernel[9];
		float m_DivFactor;
	};
}