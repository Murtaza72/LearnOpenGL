#include "glpch.h"

#include "TestBloom.h"
//
//namespace test {
//
//	TestBloom::TestBloom()
//		:
//		m_HDRFBO(SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),
//		m_WoodTex("res", "textures/wood.png", aiTextureType_DIFFUSE),
//		m_ContainerTex("res", "textures/container.png", aiTextureType_DIFFUSE),
//		m_HDRShader("res/shaders/Lighting/Advanced/bloom/hdr.vs.glsl", "res/shaders/Lighting/Advanced/bloom/hdr.fs.glsl"),
//		m_BlurShader("res/shaders/Lighting/Advanced/bloom/blur.vs.glsl", "res/shaders/Lighting/Advanced/bloom/blur.fs.glsl"),
//		m_BloomShader("res/shaders/Lighting/Advanced/bloom/bloom.vs.glsl", "res/shaders/Lighting/Advanced/bloom/bloom.fs.glsl"),
//		m_LightCubeShader("res/shaders/Lighting/Advanced/bloom/hdr.vs.glsl", "res/shaders/Lighting/Advanced/bloom/light_cube.fs.glsl"),
//		m_Bloom(true),
//		m_Exposure(1.0f)
//	{
//		glEnable(GL_DEPTH_TEST);
//
//		//glGenFramebuffers(1, &m_HDRFBO);
//		//glBindFramebuffer(GL_FRAMEBUFFER, m_HDRFBO);
//
//		m_HDRFBO.AllocateAndAttachTexture(GL_COLOR_ATTACHMENT0, GL_RGB16F, GL_RGB, GL_FLOAT,
//										  GL_LINEAR,
//										  GL_LINEAR,
//										  GL_CLAMP_TO_EDGE,
//										  GL_CLAMP_TO_EDGE);
//
//		// using Multiple Render Targets to attach two color buffer to a framebuffer
//		//glGenTextures(2, colorBuffers);
//		//for (unsigned int i = 0; i < 2; i++)
//		//{
//		//	glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
//		//	glTexImage2D(
//		//		GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL
//		//	);
//		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//		//	// attach texture to framebuffer
//		//	glFramebufferTexture2D(
//		//		GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0
//		//	);
//		//}
//
//		unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
//		glDrawBuffers(2, attachments);
//
//		m_HDRFBO.AllocateAndAttachRBO(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT);
//
//		/*unsigned int rboDepth;
//		glGenRenderbuffers(1, &rboDepth);
//		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
//		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
//		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);*/
//
//		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//			std::cout << "Framebuffer not complete!" << std::endl;
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//		/*glGenFramebuffers(2, pingpongFBO);
//		glGenTextures(2, pingpongColorbuffers);
//		*/
//		for (unsigned int i = 0; i < 2; i++)
//		{
//			m_PingpongFBO[i].AllocateAndAttachTexture(GL_COLOR_ATTACHMENT0, GL_RGB16F, GL_RGB, GL_FLOAT,
//													  GL_LINEAR,
//													  GL_LINEAR,
//													  GL_CLAMP_TO_EDGE,
//													  GL_CLAMP_TO_EDGE);
//
//			//glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
//			//glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
//			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
//			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//			//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
//
//			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//				std::cout << "Framebuffer not complete!" << std::endl;
//		}
//
//		m_LightPositions.push_back(glm::vec3(0.0f, 0.5f, 1.5f));
//		m_LightPositions.push_back(glm::vec3(-4.0f, 0.5f, -3.0f));
//		m_LightPositions.push_back(glm::vec3(3.0f, 0.5f, 1.0f));
//		m_LightPositions.push_back(glm::vec3(-.8f, 2.4f, -1.0f));
//
//		m_LightColors.push_back(glm::vec3(5.0f, 5.0f, 5.0f));
//		m_LightColors.push_back(glm::vec3(10.0f, 0.0f, 0.0f));
//		m_LightColors.push_back(glm::vec3(0.0f, 0.0f, 15.0f));
//		m_LightColors.push_back(glm::vec3(0.0f, 5.0f, 0.0f));
//
//		m_WoodTex.Load(GL_RGB);
//		m_ContainerTex.Load(GL_SRGB);
//
//		m_HDRShader.use();
//		m_HDRShader.setInt("diffuseTexture", 0);
//
//		m_BlurShader.use();
//		m_BlurShader.setInt("image", 0);
//
//		m_BloomShader.use();
//		m_BloomShader.setInt("scene", 0);
//		m_BloomShader.setInt("bloomBlur", 1);
//	}
//
//	TestBloom::~TestBloom()
//	{
//	}
//
//	void TestBloom::OnRender(Camera camera)
//	{
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		// 1. render scene into floating point framebuffer
//
//		/*glBindFramebuffer(GL_FRAMEBUFFER, m_HDRFBO);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
//
//		m_HDRFBO.Activate();
//
//		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
//		glm::mat4 view = camera.GetViewMatrix();
//		glm::mat4 model = glm::mat4(1.0f);
//
//		m_HDRShader.use();
//		m_HDRShader.setMat4("projection", projection);
//		m_HDRShader.setMat4("view", view);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, m_WoodTex.Id);
//
//		// set lighting uniforms
//		for (unsigned int i = 0; i < m_LightPositions.size(); i++)
//		{
//			m_HDRShader.setVec3("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
//			m_HDRShader.setVec3("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
//		}
//
//		m_HDRShader.setVec3("viewPos", camera.Position);
//
//		// create one large cube that acts as the floor
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0));
//		model = glm::scale(model, glm::vec3(12.5f, 0.5f, 12.5f));
//		m_HDRShader.setMat4("model", model);
//		RenderCube();
//
//		// then create multiple cubes as the scenery
//		glBindTexture(GL_TEXTURE_2D, m_ContainerTex.Id);
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
//		model = glm::scale(model, glm::vec3(0.5f));
//		m_HDRShader.setMat4("model", model);
//		RenderCube();
//
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
//		model = glm::scale(model, glm::vec3(0.5f));
//		m_HDRShader.setMat4("model", model);
//		RenderCube();
//
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(-1.0f, -1.0f, 2.0));
//		model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
//		m_HDRShader.setMat4("model", model);
//		RenderCube();
//
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(0.0f, 2.7f, 4.0));
//		model = glm::rotate(model, glm::radians(23.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
//		model = glm::scale(model, glm::vec3(1.25));
//		m_HDRShader.setMat4("model", model);
//		RenderCube();
//
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(-2.0f, 1.0f, -3.0));
//		model = glm::rotate(model, glm::radians(124.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
//		m_HDRShader.setMat4("model", model);
//		RenderCube();
//
//		model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(-3.0f, 0.0f, 0.0));
//		model = glm::scale(model, glm::vec3(0.5f));
//		m_HDRShader.setMat4("model", model);
//		RenderCube();
//
//		// finally show all the light sources as bright cubes
//		m_LightCubeShader.use();
//		m_LightCubeShader.setMat4("projection", projection);
//		m_LightCubeShader.setMat4("view", view);
//
//		for (unsigned int i = 0; i < m_LightPositions.size(); i++)
//		{
//			model = glm::mat4(1.0f);
//			model = glm::translate(model, glm::vec3(m_LightPositions[i]));
//			model = glm::scale(model, glm::vec3(0.25f));
//			m_LightCubeShader.setMat4("model", model);
//			m_LightCubeShader.setVec3("lightColor", m_LightColors[i]);
//			RenderCube();
//		}
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//		// 2. blur bright fragments with two-pass Gaussian Blur 
//
//		bool horizontal = true, first_iteration = true;
//		unsigned int amount = 10;
//		m_BlurShader.use();
//		glActiveTexture(GL_TEXTURE0);
//		for (unsigned int i = 0; i < amount; i++)
//		{
//			//glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
//			m_PingpongFBO[horizontal].Activate();
//			m_BlurShader.setInt("horizontal", horizontal);
//			glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
//			RenderQuad();
//			horizontal = !horizontal;
//			if (first_iteration)
//				first_iteration = false;
//		}
//		glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//		// 3. now render floating point color buffer to 2D quad and tonemap HDR colors to default framebuffer's (clamped) color range
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		m_BloomShader.use();
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
//
//		m_BloomShader.setBool("bloom", m_Bloom);
//		m_BloomShader.setFloat("exposure", m_Exposure);
//
//		RenderQuad();
//	}
//
//	void TestBloom::OnImGuiRender()
//	{
//		ImGui::DragFloat("Exposure", &m_Exposure, 0.1f, 0.0f, 10.0f);
//		ImGui::Checkbox("Bloom", &m_Bloom);
//	}
//}