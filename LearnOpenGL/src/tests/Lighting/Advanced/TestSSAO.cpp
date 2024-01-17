#include "glpch.h"

#include "TestSSAO.h"

#include <random>

namespace test {

	TestSSAO::TestSSAO()
		:
		m_GBuffer(SCREEN_WIDTH, SCREEN_WIDTH, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),
		m_SSAOFBO(SCREEN_WIDTH, SCREEN_WIDTH, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),
		m_NoiseTex("SSAO Noise Texture")
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		// Position
		m_GBuffer.AllocateAndAttachTexture(GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_RGBA, GL_FLOAT,
										   GL_NEAREST,
										   GL_NEAREST);

		// Normal
		m_GBuffer.AllocateAndAttachTexture(GL_COLOR_ATTACHMENT1, GL_RGBA16F, GL_RGBA, GL_FLOAT,
										   GL_NEAREST,
										   GL_NEAREST);

		// Albedo + Specular
		m_GBuffer.AllocateAndAttachTexture(GL_COLOR_ATTACHMENT2, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
										   GL_NEAREST,
										   GL_NEAREST);

		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		GLCall(glDrawBuffers(3, attachments));

		m_GBuffer.AllocateAndAttachRBO(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT);
		m_GBuffer.IsComplete();

		// SSAO Framebuffer
		m_SSAOFBO.AllocateAndAttachTexture(GL_COLOR_ATTACHMENT0, GL_RED, GL_RED, GL_FLOAT,
										   GL_NEAREST,
										   GL_NEAREST);

		std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between [0.0, 1.0]
		std::default_random_engine generator;

		for (unsigned int i = 0; i < 64; ++i)
		{
			glm::vec3 sample(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator)
			);
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);

			float scale = (float)i / 64.0;
			scale = lerp(0.1f, 1.0f, scale * scale);
			sample *= scale;
			m_SSAOKernel.push_back(sample);
		}

		for (unsigned int i = 0; i < 16; i++)
		{
			glm::vec3 noise(
				randomFloats(generator) * 2.0 - 1.0,
				randomFloats(generator) * 2.0 - 1.0,
				0.0f);
			m_SSAONoise.push_back(noise);
		}

		m_NoiseTex.Allocate(GL_RGBA16F, GL_RGB, 4, 4, GL_FLOAT, &m_SSAONoise[0]);
		m_NoiseTex.SetParams(GL_NEAREST, GL_NEAREST, GL_REPEAT, GL_REPEAT);
	}

	TestSSAO::~TestSSAO()
	{

	}

	void TestSSAO::OnRender(Camera camera)
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_GBuffer.Activate();
		// [...]
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// use G-buffer to render SSAO texture
		m_SSAOFBO.Activate();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_NoiseTex.Id);

		shaderSSAO.use();
		SendKernelSamplesToShader();
		shaderSSAO.setMat4("projection", projection);
		RenderQuad();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// lighting pass: render scene lighting
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderLightingPass.use();
		//[...]
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		//[...]
		RenderQuad();
	}

	void TestSSAO::OnImGuiRender()
	{

	}
}