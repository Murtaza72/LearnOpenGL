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
		Framebuffer m_HDRFBO;
		Shader m_Shader, m_HDRShader;
		Texture m_WoodTex;
		std::vector<glm::vec3> m_LightPositions, m_LightColors;
		float m_Exposure;
		bool m_Tone;
	};
}
