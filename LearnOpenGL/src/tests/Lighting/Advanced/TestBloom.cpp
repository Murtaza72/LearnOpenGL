#include "glpch.h"

#include "TestBloom.h"

namespace test {

	TestBloom::TestBloom()
		:
		m_HDRFBO(SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),
		m_PingpongFBO
		{
			Framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),
			Framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
		},
		m_WoodTex("res", "textures/wood.png", aiTextureType_DIFFUSE),
		m_ContainerTex("res", "textures/container.png", aiTextureType_DIFFUSE),
		m_HDRShader("res/shaders/Lighting/Advanced/bloom/hdr.vs.glsl", "res/shaders/Lighting/Advanced/bloom/hdr.fs.glsl"),
		m_BlurShader("res/shaders/Lighting/Advanced/bloom/blur.vs.glsl", "res/shaders/Lighting/Advanced/bloom/blur.fs.glsl"),
		m_BloomShader("res/shaders/Lighting/Advanced/bloom/bloom.vs.glsl", "res/shaders/Lighting/Advanced/bloom/bloom.fs.glsl"),
		m_LightCubeShader("res/shaders/Lighting/Advanced/bloom/hdr.vs.glsl", "res/shaders/Lighting/Advanced/bloom/light_cube.fs.glsl"),
		m_Bloom(true),
		m_Exposure(1.0f)
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		m_HDRFBO.Bind();
		for (unsigned int i = 0; i < 2; i++)
		{
			m_HDRFBO.AllocateAndAttachTexture(GL_COLOR_ATTACHMENT0 + i, GL_RGB16F, GL_RGB, GL_FLOAT,
											  GL_LINEAR,
											  GL_LINEAR,
											  GL_CLAMP_TO_EDGE,
											  GL_CLAMP_TO_EDGE);
		}

		// using Multiple Render Targets to attach two color buffer to a framebuffer
		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		GLCall(glDrawBuffers(2, attachments));

		m_HDRFBO.AllocateAndAttachRBO(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT);

		m_HDRFBO.IsComplete();
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		for (unsigned int i = 0; i < 2; i++)
		{
			m_PingpongFBO[i].Bind();
			m_PingpongFBO[i].AllocateAndAttachTexture(GL_COLOR_ATTACHMENT0, GL_RGB16F, GL_RGB, GL_FLOAT,
													  GL_LINEAR,
													  GL_LINEAR,
													  GL_CLAMP_TO_EDGE,
													  GL_CLAMP_TO_EDGE);

			m_PingpongFBO[i].IsComplete();
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		m_LightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
		m_LightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
		m_LightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
		m_LightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));

		m_LightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
		m_LightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
		m_LightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
		m_LightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));

		m_WoodTex.Load(GL_RGB);
		m_ContainerTex.Load(GL_SRGB);

		m_HDRShader.use();
		m_HDRShader.setInt("diffuseTexture", 0);

		m_BlurShader.use();
		m_BlurShader.setInt("image", 0);

		m_BloomShader.use();
		m_BloomShader.setInt("scene", 0);
		m_BloomShader.setInt("bloomBlur", 1);
	}

	TestBloom::~TestBloom()
	{
		m_HDRFBO.Destroy();
		m_PingpongFBO[0].Destroy();
		m_PingpongFBO[1].Destroy();
	}

	void TestBloom::OnRender(Camera camera)
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		m_HDRFBO.Activate();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		m_HDRShader.use();
		m_HDRShader.setMat4("projection", projection);
		m_HDRShader.setMat4("view", view);
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_WoodTex.Id));

		// set lighting uniforms
		for (unsigned int i = 0; i < m_LightPositions.size(); i++)
		{
			m_HDRShader.setVec3("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
			m_HDRShader.setVec3("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
		}

		m_HDRShader.setVec3("viewPos", camera.Position);

		// create one large cube that acts as the floor
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
		model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
		m_HDRShader.setMat4("model", model);
		RenderCube();

		// then create multiple cubes as the scenery
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ContainerTex.Id));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		m_HDRShader.setMat4("model", model);
		RenderCube();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
		model = glm::scale(model, glm::vec3(0.5f));
		m_HDRShader.setMat4("model", model);
		RenderCube();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		m_HDRShader.setMat4("model", model);
		RenderCube();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
		model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		model = glm::scale(model, glm::vec3(1.25));
		m_HDRShader.setMat4("model", model);
		RenderCube();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
		model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
		m_HDRShader.setMat4("model", model);
		RenderCube();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.5f));
		m_HDRShader.setMat4("model", model);
		RenderCube();

		// finally show all the light sources as bright cubes
		m_LightCubeShader.use();
		m_LightCubeShader.setMat4("projection", projection);
		m_LightCubeShader.setMat4("view", view);

		for (unsigned int i = 0; i < m_LightPositions.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(m_LightPositions[i]));
			model = glm::scale(model, glm::vec3(0.25f));
			m_LightCubeShader.setMat4("model", model);
			m_LightCubeShader.setVec3("lightColor", m_LightColors[i]);
			RenderCube();
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		// 2. blur bright fragments with two-pass Gaussian Blur 

		bool horizontal = true, first_iteration = true;
		unsigned int amount = 1;
		m_BlurShader.use();
		GLCall(glActiveTexture(GL_TEXTURE0));
		for (unsigned int i = 0; i < amount; i++)
		{
			m_PingpongFBO[horizontal].Activate();
			m_BlurShader.setInt("horizontal", horizontal);
			GLCall(glBindTexture(GL_TEXTURE_2D, first_iteration ? m_HDRFBO.GetTexture(0)
								 : m_PingpongFBO[!horizontal].GetTexture(0)));  // bind texture of other framebuffer (or scene if first iteration)
			RenderQuad();
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		// 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_BloomShader.use();
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_HDRFBO.GetTexture(0)));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_PingpongFBO[!horizontal].GetTexture(0)));

		m_BloomShader.setBool("bloom", m_Bloom);
		m_BloomShader.setFloat("exposure", m_Exposure);

		RenderQuad();

		m_HDRFBO.Bind();
		DisplayFramebufferTexture(m_HDRFBO.GetTexture(0));
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void TestBloom::OnImGuiRender()
	{
		ImGui::DragFloat("Exposure", &m_Exposure, 0.1f, 0.0f, 10.0f);
		ImGui::Checkbox("Bloom", &m_Bloom);
	}
}