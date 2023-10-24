#include "glpch.h"

#include "TestHDR.h"

namespace test {

	TestHDR::TestHDR()
		:
		m_HDRFBO(0),
		m_Exposure(1.0f),
		m_Tone(true),
		m_WoodTex("res", "textures/wood.png", aiTextureType_DIFFUSE),
		m_HDRShader("res/shaders/Lighting/Advanced/hdr/hdr.vs.glsl", "res/shaders/Lighting/Advanced/hdr/hdr.fs.glsl"),
		m_Shader("res/shaders/Lighting/Advanced/hdr/quad.vs.glsl", "res/shaders/Lighting/Advanced/hdr/quad.fs.glsl")
	{
		glEnable(GL_DEPTH_TEST);

		glGenFramebuffers(1, &m_HDRFBO);

		// create floating point color buffer
		glGenTextures(1, &m_ColorBuffer);
		glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// create depth buffer (renderbuffer)
		unsigned int rboDepth;
		glGenRenderbuffers(1, &rboDepth);
		glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);

		// attach buffers
		glBindFramebuffer(GL_FRAMEBUFFER, m_HDRFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorBuffer, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_WoodTex.Load(GL_SRGB);

		m_Shader.use();
		m_Shader.setInt("diffuseTexture", 0);
		m_HDRShader.use();
		m_HDRShader.setInt("hdrBuffer", 0);

		m_LightPositions.push_back(glm::vec3(0.0f, 0.0f, 49.5f)); // back light
		m_LightPositions.push_back(glm::vec3(-1.4f, -1.9f, 9.0f));
		m_LightPositions.push_back(glm::vec3(0.0f, -1.8f, 4.0f));
		m_LightPositions.push_back(glm::vec3(0.8f, -1.7f, 6.0f));

		m_LightColors.push_back(glm::vec3(200.0f, 200.0f, 200.0f));
		m_LightColors.push_back(glm::vec3(0.1f, 0.0f, 0.0f));
		m_LightColors.push_back(glm::vec3(0.0f, 0.0f, 0.2f));
		m_LightColors.push_back(glm::vec3(0.0f, 0.1f, 0.0f));
	}

	TestHDR::~TestHDR()
	{
	}

	void TestHDR::OnRender(Camera camera)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_HDRFBO);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		m_Shader.use();
		m_Shader.setMat4("projection", projection);
		m_Shader.setMat4("view", view);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_WoodTex.Id);

		for (unsigned int i = 0; i < m_LightPositions.size(); i++)
		{
			m_Shader.setVec3("lights[" + std::to_string(i) + "].Position", m_LightPositions[i]);
			m_Shader.setVec3("lights[" + std::to_string(i) + "].Color", m_LightColors[i]);
		}
		m_Shader.setVec3("viewPos", camera.Position);

		// render tunnel
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 25.0));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 27.5f));
		m_Shader.setMat4("model", model);
		m_Shader.setInt("inverse_normals", true);
		RenderCube();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_HDRShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ColorBuffer);
		//m_HDRShader.setInt("hdr", hdr);
		m_HDRShader.setFloat("exposure", m_Exposure);
		m_HDRShader.setBool("tone", m_Tone);
		RenderQuad();
	}

	void TestHDR::OnImGuiRender()
	{
		ImGui::DragFloat("Exposure", &m_Exposure, 0.1f, 0.0f, 10.0f);
		ImGui::Checkbox("Tone", &m_Tone);
	}
}