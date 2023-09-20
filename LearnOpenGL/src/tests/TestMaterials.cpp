#include "TestMaterials.h"

#include <GLFW/glfw3.h>

#include "imgui/imgui.h"

namespace test {

	TestMaterials::TestMaterials()
		:
		m_VBO(0), m_CubeVAO(0), m_LightCubeVAO(0),
		m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
		m_LightPos{ 1.2f, 1.0f, 2.0f },
		m_ObjectColor{ 1.0f, 0.5f, 0.31f },
		m_LightColor{ 1.0f,1.0f,1.0f },
		m_AmbientColor{ 0.0f, 0.0f, 0.0f },
		m_DiffuseColor{ 0.0f, 0.0f, 0.0f },
		m_Shininess(32.0f),
		m_LightingShader("res/shaders/materials.vs.glsl", "res/shaders/materials.fs.glsl"),
		m_LightCubeShader("res/shaders/light_cube.vs.glsl", "res/shaders/light_cube.fs.glsl")
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		GLCall(GLCall(glGenBuffers(1, &m_VBO)));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

		// Base Cube
		GLCall(glGenVertexArrays(1, &m_CubeVAO));
		GLCall(glBindVertexArray(m_CubeVAO));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0));

		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)(3 * sizeof(float))));

		// Light Cube
		GLCall(glGenVertexArrays(1, &m_LightCubeVAO));
		GLCall(glBindVertexArray(m_LightCubeVAO));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (const void*)0));
	}

	TestMaterials::~TestMaterials()
	{
		GLCall(glDisable(GL_DEPTH_TEST));

		GLCall(glDeleteBuffers(1, &m_VBO));
		GLCall(glDeleteVertexArrays(1, &m_CubeVAO));
		GLCall(glDeleteVertexArrays(1, &m_LightCubeVAO));

	}

	void TestMaterials::OnRender(Camera camera)
	{
		m_Camera = camera;

		/*m_LightPos.x = 2.0f * sin(GLCall(glfwGetTime()));
		m_LightPos.y = -0.3f;
		m_LightPos.z = 1.5f * cos(GLCall(glfwGetTime()));*/

		m_LightingShader.use();
		m_LightingShader.setVec3("objectColor", m_ObjectColor);
		m_LightingShader.setVec3("lightColor", m_LightColor);
		m_LightingShader.setVec3("lightPos", m_LightPos);
		m_LightingShader.setVec3("viewPos", m_Camera.Position);

		// Lighting
		m_LightingShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		m_LightingShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		m_LightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		m_LightingShader.setFloat("material.shininess", m_Shininess);

		glm::vec3 lightColor(0.0f);
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		m_DiffuseColor = lightColor * glm::vec3(0.5f);
		m_AmbientColor = m_DiffuseColor * glm::vec3(0.2f);

		m_LightingShader.setVec3("light.ambient", m_AmbientColor);
		m_LightingShader.setVec3("light.diffuse", m_DiffuseColor);
		m_LightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(m_Camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		m_LightingShader.setMat4("projection", projection);
		glm::mat4 view = m_Camera.GetViewMatrix();
		m_LightingShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		m_LightingShader.setMat4("model", model);

		GLCall(glBindVertexArray(m_CubeVAO));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		// draw light cube
		m_LightCubeShader.use();
		m_LightCubeShader.setMat4("projection", projection);
		m_LightCubeShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, m_LightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		m_LightCubeShader.setMat4("model", model);

		GLCall(glBindVertexArray(m_LightCubeVAO));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}

	void TestMaterials::OnImGuiRender()
	{
		ImGui::SliderFloat3("Light Position", &m_LightPos.x, -5.0f, 5.0f);
		ImGui::SliderFloat3("Ambient", &m_DiffuseColor.x, -0.5f, 0.5f);
		ImGui::SliderFloat3("Diffuse", &m_AmbientColor.x, -0.2f, .2f);
		ImGui::SliderFloat("Shininess", &m_Shininess, 0.0f, 64.0f);
	}
}