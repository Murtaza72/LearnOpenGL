#include "TestPhongLighting.h"

namespace test {

	TestPhongLighting::TestPhongLighting()
		:
		m_LightPos{ 1.2f, 1.0f, 2.0f },
		m_LightingShader{ "res/shaders/Lighting/Basic/phong_colors.vs.glsl", "res/shaders/Lighting/Basic/phong_colors.fs.glsl" },
		m_LightCubeShader{ "res/shaders/Lighting/Basic/light_cube.vs.glsl", "res/shaders/Lighting/Basic/light_cube.fs.glsl" },
		m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
		m_CubeVAO(0),
		m_VBO(0),
		m_LightCubeVAO(0),
		m_ObjectColor{ 1.0f, 0.5f, 0.31f },
		m_LightColor{ 1.0f,1.0f,1.0f },
		m_RotateCube(0)
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

		GLCall(glGenBuffers(1, &m_VBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

		GLCall(glGenVertexArrays(1, &m_CubeVAO));
		GLCall(glBindVertexArray(m_CubeVAO));

		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
		GLCall(glEnableVertexAttribArray(0));

		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
		GLCall(glEnableVertexAttribArray(1));

		GLCall(glGenVertexArrays(1, &m_LightCubeVAO));
		GLCall(glBindVertexArray(m_LightCubeVAO));

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));

		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0));
		GLCall(glEnableVertexAttribArray(0));
	}

	TestPhongLighting::~TestPhongLighting()
	{
		GLCall(glDisable(GL_DEPTH_TEST));

		GLCall(glDeleteVertexArrays(1, &m_CubeVAO));
		GLCall(glDeleteVertexArrays(1, &m_LightCubeVAO));
		GLCall(glDeleteBuffers(1, &m_VBO));
	}

	void TestPhongLighting::OnRender(Camera camera)
	{
		m_Camera = camera;

		if (m_RotateCube)
		{
			m_LightPos.x = 2.0f * sin(glfwGetTime());
			m_LightPos.z = 1.5f * cos(glfwGetTime());
		}

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

		GLCall(glBindVertexArray(m_CubeVAO));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		// draw light cube
		m_LightCubeShader.use();
		m_LightCubeShader.setMat4("projection", projection);
		m_LightCubeShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, m_LightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		m_LightCubeShader.setMat4("model", model);

		GLCall(glBindVertexArray(m_LightCubeVAO));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}

	void TestPhongLighting::OnImGuiRender()
	{
		ImGui::SliderFloat3("Light Position", &m_LightPos.x, -5.0f, 5.0f);
		ImGui::SliderInt("Rotate", &m_RotateCube, 0, 1);
		ImGui::ColorPicker3("Light Color", &m_LightColor.x);
		ImGui::ColorPicker3("Object Color", &m_ObjectColor.x);

	}
}