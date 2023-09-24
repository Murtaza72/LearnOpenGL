#include "TestLightCasters.h"

namespace test {

	TestLightCasters::TestLightCasters()
		:
		m_VBO(0), m_CubeVAO(0), m_LightCubeVAO(0),
		m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
		m_LightPos{ 1.2f, 1.0f, 2.0f },
		m_Shininess(32.0f),
		m_DiffuseMap(0),
		m_RotateCube(0),
		m_RotationSpeed(20.0f),
		m_Radius(2.0f),
		m_LightingShader("res/shaders/Lighting/Basic/light_casters.vs.glsl", "res/shaders/Lighting/Basic/light_casters.fs.glsl"),
		m_LightCubeShader("res/shaders/Lighting/Basic/light_cube.vs.glsl", "res/shaders/Lighting/Basic/light_cube.fs.glsl")
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		float vertices[] = {
			// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
		};

		m_CubePositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		m_CubePositions.push_back(glm::vec3(2.0f, 5.0f, -15.0f));
		m_CubePositions.push_back(glm::vec3(-1.5f, -2.2f, -2.5f));
		m_CubePositions.push_back(glm::vec3(-3.8f, -2.0f, -12.3f));
		m_CubePositions.push_back(glm::vec3(2.4f, -0.4f, -3.5f));
		m_CubePositions.push_back(glm::vec3(-1.7f, 3.0f, -7.5f));
		m_CubePositions.push_back(glm::vec3(1.3f, -2.0f, -2.5f));
		m_CubePositions.push_back(glm::vec3(1.5f, 2.0f, -2.5f));
		m_CubePositions.push_back(glm::vec3(1.5f, 0.2f, -1.5f));
		m_CubePositions.push_back(glm::vec3(-1.3f, 1.0f, -1.5f));

		GLCall(glGenBuffers(1, &m_VBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

		// Base Cube
		GLCall(glGenVertexArrays(1, &m_CubeVAO));
		GLCall(glBindVertexArray(m_CubeVAO));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)0));

		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float))));

		GLCall(glEnableVertexAttribArray(2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(6 * sizeof(float))));

		// Light Cube
		GLCall(glGenVertexArrays(1, &m_LightCubeVAO));
		GLCall(glBindVertexArray(m_LightCubeVAO));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)0));

		m_DiffuseMap = LoadTexture("res/textures/container.png");
		m_SpecularMap = LoadTexture("res/textures/container_specular.png");

		m_LightingShader.use();
		m_LightingShader.setInt("material.diffuse", 0);
		m_LightingShader.setInt("material.specular", 1);
	}

	TestLightCasters::~TestLightCasters()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glDisable(GL_BLEND));

		GLCall(glDeleteBuffers(1, &m_VBO));
		GLCall(glDeleteVertexArrays(1, &m_CubeVAO));
		GLCall(glDeleteVertexArrays(1, &m_LightCubeVAO));

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void TestLightCasters::OnRender(Camera camera)
	{
		m_Camera = camera;

		if (m_RotateCube)
		{
			m_LightPos.x = m_Radius * sin(glfwGetTime());
			m_LightPos.z = m_Radius * cos(glfwGetTime());
		}

		m_LightingShader.use();
		m_LightingShader.setVec3("light.position", camera.Position);
		m_LightingShader.setVec3("light.direction", camera.Front);
		m_LightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
		m_LightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
		m_LightingShader.setVec3("viewPos", camera.Position);

		// light properties
		m_LightingShader.setVec3("light.ambient", glm::vec3(0.1f));
		m_LightingShader.setVec3("light.diffuse", glm::vec3(0.8f));
		m_LightingShader.setVec3("light.specular", glm::vec3(1.0f));
		m_LightingShader.setFloat("light.constant", 1.0f);
		m_LightingShader.setFloat("light.linear", 0.09f);
		m_LightingShader.setFloat("light.quadratic", 0.032f);

		// material properties
		m_LightingShader.setFloat("material.shininess", m_Shininess);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		m_LightingShader.setMat4("projection", projection);
		m_LightingShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		m_LightingShader.setMat4("model", model);

		// bind diffuse map
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_DiffuseMap));

		// binding specular map
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_SpecularMap));

		// render the cube
		glBindVertexArray(m_CubeVAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, m_CubePositions[i]);
			float angle = sin(glfwGetTime()) * m_RotationSpeed * (i + 10);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			m_LightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// draw light cube
		/*m_LightCubeShader.use();
		m_LightCubeShader.setMat4("projection", projection);
		m_LightCubeShader.setMat4("view", view);
		model = glm::mat4(1.0f);
		model = glm::translate(model, m_LightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		m_LightCubeShader.setMat4("model", model);*/

		//GLCall(glBindVertexArray(m_LightCubeVAO));
		//GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
	}

	void TestLightCasters::OnImGuiRender()
	{
		ImGui::SliderFloat3("Light Position", &m_LightPos.x, -5.0f, 5.0f);
		ImGui::SliderFloat("Shininess", &m_Shininess, 0.0f, 64.0f);
		ImGui::SliderFloat("Rotation Speed", &m_RotationSpeed, 0.0f, 100.0f);
		ImGui::SliderInt("Rotate", &m_RotateCube, 0, 1);
		ImGui::SliderFloat("Radius", &m_Radius, 0.0f, 20.0f);
	}
}