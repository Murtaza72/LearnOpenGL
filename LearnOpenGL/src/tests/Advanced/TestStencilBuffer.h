#pragma once

#include "tests/Test.h"

namespace  test {

	class TestStencilBuffer : public Test
	{
	public:
		TestStencilBuffer();
		~TestStencilBuffer();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_CubeVAO, m_CubeVBO, m_PlaneVAO, m_PlaneVBO;
		unsigned int m_CubeTexture, m_FloorTexture;
		Shader m_Shader;
		Shader m_OutlineShader;
		float m_Scale;
	};
}