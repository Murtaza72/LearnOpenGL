#pragma once

#include "tests/Test.h"

#include "Framebuffer.h"

namespace test {

	class TestSSAO : public Test
	{
	public:
		TestSSAO();
		~TestSSAO();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

		inline float lerp(float a, float b, float f)
		{
			return a + f * (b - a);
		}

	private:
		Framebuffer m_GBuffer, m_SSAOFBO;
		Texture m_NoiseTex;

		std::vector<glm::vec3> m_SSAOKernel;
		std::vector<glm::vec3> m_SSAONoise;
	};
}
