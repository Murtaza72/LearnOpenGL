#include "glpch.h"

#include "TestParallaxMapping.h"

namespace test {

	TestParallaxMapping::TestParallaxMapping()
		:
		m_VAO(0),
		m_HeightScale(0.1f),
		m_LightPos({ 0.5f, 1.0f, 0.3f }),
		m_DiffuseTex("res", "textures/bricks.jpg", aiTextureType_DIFFUSE),
		m_NormalTex("res", "textures/bricks_normal.jpg", aiTextureType_NORMALS),
		m_ParallaxTex("res", "textures/bricks_disp.jpg", aiTextureType_DIFFUSE),
		m_Shader("res/shaders/Lighting/Advanced/parallax_mapping.vs.glsl", "res/shaders/Lighting/Advanced/parallax_mapping.fs.glsl")
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		m_VAO = GenerateTangents();

		m_DiffuseTex.Load(GL_RGB);
		m_NormalTex.Load(GL_RGB);
		m_ParallaxTex.Load(GL_RGB);

		m_Shader.use();
		m_Shader.setInt("diffuseMap", 0);
		m_Shader.setInt("normalMap", 1);
		m_Shader.setInt("depthMap", 2);
	}

	TestParallaxMapping::~TestParallaxMapping()
	{

	}

	void TestParallaxMapping::OnRender(Camera camera)
	{
		m_Shader.use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime() * -1.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));

		m_Shader.setMat4("model", model);
		m_Shader.setMat4("projection", projection);
		m_Shader.setMat4("view", view);

		m_Shader.setVec3("viewPos", camera.Position);
		m_Shader.setVec3("lightPos", m_LightPos);
		m_Shader.setFloat("heightScale", m_HeightScale);

		GLCall(glBindVertexArray(m_VAO));
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_DiffuseTex.Id));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_NormalTex.Id));
		GLCall(glActiveTexture(GL_TEXTURE2));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_ParallaxTex.Id));

		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

		// renders small light cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, m_LightPos);
		model = glm::scale(model, glm::vec3(0.1f));
		m_Shader.setMat4("model", model);

		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6))
	}

	void TestParallaxMapping::OnImGuiRender()
	{
		ImGui::DragFloat3("Light Pos", &m_LightPos.x, 1.0f, -10.0f, 10.0f);
		ImGui::DragFloat("Height Scale", &m_HeightScale, 0.1f, 0.0f, 10.0f);
	}
}