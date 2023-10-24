#pragma once

#include "tests/Test.h"

#include "Framebuffer.h"

namespace test {

	class TestHDR : public Test
	{
	public:
		TestHDR();
		~TestHDR();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_HDRFBO;
		unsigned int m_ColorBuffer;
		Shader m_Shader, m_HDRShader;
		Texture m_WoodTex;
		std::vector<glm::vec3> m_LightPositions, m_LightColors;
		float m_Exposure;
		bool m_Tone;
	};
}
