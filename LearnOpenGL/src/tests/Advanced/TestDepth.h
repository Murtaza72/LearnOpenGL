#pragma once

#include "tests/Test.h"

namespace  test {


	class TestDepth : public Test
	{
	public:
		TestDepth();
		~TestDepth();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_CubeVAO, m_CubeVBO, m_PlaneVAO, m_PlaneVBO;
		unsigned int m_CubeTexture, m_FloorTexture;
		Shader m_Shader;
	};
}