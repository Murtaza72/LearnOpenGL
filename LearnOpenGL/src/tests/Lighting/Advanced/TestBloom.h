#pragma once

#include "tests/Test.h"

namespace test {

	class TestBloom : public Test
	{
	public:
		TestBloom();
		~TestBloom();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_HDRFBO;
		unsigned int colorBuffers[2], pingpongColorbuffers[2], pingpongFBO[2];
		Shader m_HDRShader, m_BlurShader, m_BloomShader, m_LightCubeShader;
		Texture m_WoodTex, m_ContainerTex;

		std::vector<glm::vec3> m_LightPositions, m_LightColors;
		bool m_Bloom;
		float m_Exposure;
	};
}
