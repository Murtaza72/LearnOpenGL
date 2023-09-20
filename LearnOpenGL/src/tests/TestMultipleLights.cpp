#include "TestMultipleLights.h"

#include <string>

namespace test
{
	TestMultipleLights::TestMultipleLights()
		:
		m_VBO(0), m_CubeVAO(0), m_LightCubeVAO(0),
		m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
		m_LightPos{ 1.2f, 1.0f, 2.0f },
		m_DiffuseMap(0),
		m_RotationSpeed(20.0f),
		m_LightingShader("res/shaders/multiple_lights.vs.glsl", "res/shaders/multiple_lights.fs.glsl"),
		m_LightCubeShader("res/shaders/multiple_lights_cube.vs.glsl", "res/shaders/multiple_lights_cube.fs.glsl"),
		m_EnableDirLight(false), m_EnablePointLight(false), m_EnableSpotLight(false),
		m_DirLight{ glm::vec3(-1.0f), glm::vec4(0.5f), glm::vec4(0.4f), glm::vec4(0.5f) },

		m_SpotLight{ glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.1f), glm::vec3(1.0f),
					 glm::vec3(1.0f), 12.5f, 15.0f }
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

		m_PointLightPos.push_back(glm::vec3(0.7f, 0.2f, 2.0f));
		m_PointLightPos.push_back(glm::vec3(2.3f, -3.3f, -4.0f));
		m_PointLightPos.push_back(glm::vec3(-4.0f, 2.0f, -12.0f));
		m_PointLightPos.push_back(glm::vec3(0.0f, 0.0f, -3.0f));

		m_PointLightColor.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
		m_PointLightColor.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		m_PointLightColor.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
		m_PointLightColor.push_back(glm::vec3(1.0f, 1.0f, 0.0f));

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

	TestMultipleLights::~TestMultipleLights()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glDisable(GL_BLEND));

		GLCall(glDeleteBuffers(1, &m_VBO));
		GLCall(glDeleteVertexArrays(1, &m_CubeVAO));
		GLCall(glDeleteVertexArrays(1, &m_LightCubeVAO));

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void TestMultipleLights::OnRender(Camera camera)
	{
		m_Camera = camera;

		m_LightingShader.use();

		// directional light
		if (m_EnableDirLight) {
			m_LightingShader.setVec3("dirLight.direction", m_DirLight.direction);
			m_LightingShader.setVec3("dirLight.ambient", m_DirLight.ambient);
			m_LightingShader.setVec3("dirLight.diffuse", m_DirLight.diffuse);
			m_LightingShader.setVec3("dirLight.specular", m_DirLight.specular);
		}

		// point lights
		for (int i = 0, size = m_PointLightPos.size(); i < size; i++) {
			std::string index = std::to_string(i);

			m_LightingShader.setVec3("pointLights[" + index + "].position", m_PointLightPos[i]);
			m_LightingShader.setVec3("pointLights[" + index + "].ambient", 0.05f, 0.05f, 0.05f);
			m_LightingShader.setVec3("pointLights[" + index + "].diffuse", 0.8f, 0.8f, 0.8f);
			m_LightingShader.setVec3("pointLights[" + index + "].specular", 1.0f, 1.0f, 1.0f);
			m_LightingShader.setFloat("pointLights[" + index + "].constant", 1.0f);
			m_LightingShader.setFloat("pointLights[" + index + "].linear", 0.09f);
			m_LightingShader.setFloat("pointLights[" + index + "].quadratic", 0.032f);
		}

		// spotLight
		if (m_EnableSpotLight) {
			m_LightingShader.setVec3("spotLight.position", camera.Position);
			m_LightingShader.setVec3("spotLight.direction", camera.Front);
			m_LightingShader.setVec3("spotLight.ambient", m_SpotLight.ambient);
			m_LightingShader.setVec3("spotLight.diffuse", m_SpotLight.diffuse);
			m_LightingShader.setVec3("spotLight.specular", m_SpotLight.specular);
			m_LightingShader.setFloat("spotLight.constant", 1.0f);
			m_LightingShader.setFloat("spotLight.linear", 0.09f);
			m_LightingShader.setFloat("spotLight.quadratic", 0.032f);
			m_LightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(m_SpotLight.cutOff)));
			m_LightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(m_SpotLight.outerCutOff)));
		}

		// material properties
		m_LightingShader.setFloat("material.shininess", 64.0f);

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
		m_LightCubeShader.use();
		m_LightCubeShader.setMat4("projection", projection);
		m_LightCubeShader.setMat4("view", view);

		glBindVertexArray(m_LightCubeVAO);
		for (int i = 0, size = m_PointLightPos.size(); i < size; i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, m_PointLightPos[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			m_LightCubeShader.setMat4("model", model);

			m_LightCubeShader.setVec3("colorIn", m_PointLightColor[i]);
			GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		}
	}

	void TestMultipleLights::OnImGuiRender()
	{
		ImGui::Begin("Controls");
		{
			ImGui::DragFloat("Rotation Speed", &m_RotationSpeed, 0.1f, 0.0f, 20.0f);
		}
		ImGui::End();

		ImGui::Begin("Directional Light");
		{
			ImGui::Checkbox("Enabled", &m_EnableDirLight);
			ImGui::DragFloat3("Direction", &m_DirLight.direction.x, 0.1f, -10.0f, 10.0f);
			ImGui::ColorEdit3("Ambient", (float*)&m_DirLight.ambient, ImGuiColorEditFlags_Float);
			ImGui::ColorEdit3("Diffuse", (float*)&m_DirLight.diffuse, ImGuiColorEditFlags_Float);
			ImGui::ColorEdit3("Specular", (float*)&m_DirLight.specular, ImGuiColorEditFlags_Float);
		}
		ImGui::End();

		ImGui::Begin("Point Light");
		{

		}
		ImGui::End();

		ImGui::Begin("Spot Light");
		{
			ImGui::Checkbox("Enabled", &m_EnableSpotLight);
			ImGui::DragFloat3("Position", &m_SpotLight.position.x, 0.1f, -10.0f, 10.0f);
			ImGui::DragFloat3("Direction", &m_SpotLight.direction.x, 0.1f, -10.0f, 10.0f);
			ImGui::ColorEdit3("Ambient", (float*)&m_SpotLight.ambient, ImGuiColorEditFlags_Float);
			ImGui::ColorEdit3("Diffuse", (float*)&m_SpotLight.diffuse, ImGuiColorEditFlags_Float);
			ImGui::ColorEdit3("Specular", (float*)&m_SpotLight.specular, ImGuiColorEditFlags_Float);
			ImGui::DragFloat("Inner Cutoff", &m_SpotLight.cutOff, 0.1f, 0.0f, 15.0f);
			ImGui::DragFloat("Outer Cutoff", &m_SpotLight.outerCutOff, 0.1f, 0.0f, 15.0f);
		}
		ImGui::End();
	}
}