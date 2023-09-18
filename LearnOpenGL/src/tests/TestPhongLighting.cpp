#include "TestPhongLighting.h"

#include <GL/glew.h>
#include "GLFW/glfw3.h"

#include "imgui/imgui.h"

namespace test
{
	TestPhongLighting::TestPhongLighting()
		:
		m_LightPos{ 1.2f, 1.0f, 2.0f },
		m_LightingShader{ "res/shaders/phong_colors.vs", "res/shaders/phong_colors.fs" },
		m_LightCubeShader{ "res/shaders/light_cube.vs", "res/shaders/light_cube.fs" },
		m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
		m_CubeVAO(0),
		m_VBO(0),
		m_LightCubeVAO(0),
		m_ObjectColor{ 1.0f, 0.5f, 0.31f },
		m_LightColor{ 1.0f,1.0f,1.0f }
	{
		glEnable(GL_DEPTH_TEST);

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

		glGenBuffers(1, &m_VBO);
		glGenVertexArrays(1, &m_CubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(m_CubeVAO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glGenVertexArrays(1, &m_LightCubeVAO);
		glBindVertexArray(m_LightCubeVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	TestPhongLighting::~TestPhongLighting()
	{
		glDisable(GL_DEPTH_TEST);

		glDeleteVertexArrays(1, &m_CubeVAO);
		glDeleteVertexArrays(1, &m_LightCubeVAO);
		glDeleteBuffers(1, &m_VBO);
	}

	void TestPhongLighting::OnUpdate()
	{
	}

	void TestPhongLighting::OnRender(Camera camera)
	{
		m_Camera = camera;

		m_LightPos.x = 2.0f * sin(glfwGetTime());
		m_LightPos.y = -0.3f;
		m_LightPos.z = 1.5f * cos(glfwGetTime());

		m_LightingShader.use();
		m_LightingShader.setVec3("objectColor", m_ObjectColor);
		m_LightingShader.setVec3("lightColor", m_LightColor);
		m_LightingShader.setVec3("lightPos", m_LightPos);
		m_LightingShader.setVec3("viewPos", m_Camera.Position);

		glm::mat4 projection = glm::perspective(glm::radians(m_Camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		m_LightingShader.setMat4("projection", projection);
		glm::mat4 view = m_Camera.GetViewMatrix();
		m_LightingShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		m_LightingShader.setMat4("model", model);

		glBindVertexArray(m_CubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// draw light cube
		m_LightCubeShader.use();
		m_LightCubeShader.setMat4("projection", projection);
		m_LightCubeShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, m_LightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		m_LightCubeShader.setMat4("model", model);

		glBindVertexArray(m_LightCubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	void TestPhongLighting::OnImGuiRender()
	{
		ImGui::SliderFloat3("Light Position", &m_LightPos.x, -5.0f, 5.0f);
		ImGui::ColorPicker3("Light Color", &m_LightColor.x);
		ImGui::ColorPicker3("Object Color", &m_ObjectColor.x);
	}
}