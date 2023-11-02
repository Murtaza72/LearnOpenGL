#include "glpch.h"

#include "TestHDR.h"

namespace test {

	TestHDR::TestHDR()
		:
		m_Exposure(1.0f),
		m_Tone(true),
		m_HDRFBO(SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),
		m_WoodTex("res", "textures/wood.png", aiTextureType_DIFFUSE),
		m_HDRShader("res/shaders/Lighting/Advanced/hdr/hdr.vs.glsl", "res/shaders/Lighting/Advanced/hdr/hdr.fs.glsl"),
		m_Shader("res/shaders/Lighting/Advanced/hdr/quad.vs.glsl", "res/shaders/Lighting/Advanced/hdr/quad.fs.glsl")
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		m_HDRFBO.AllocateAndAttachTexture(GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_RGBA, GL_FLOAT);
		m_HDRFBO.AllocateAndAttachRBO(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT);

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
		m_WoodTex.Destroy();
		m_HDRFBO.Destroy();
	}

	void TestHDR::OnRender(Camera camera)
	{
		m_HDRFBO.Activate();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
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
		m_HDRFBO.ActivateTexture();

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