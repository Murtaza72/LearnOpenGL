#include "glpch.h"

#include "TestLightingMaps.h"

namespace test {

	TestLightingMaps::TestLightingMaps()
		:
		m_VBO(0), m_CubeVAO(0), m_LightCubeVAO(0),
		m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)),
		m_LightPos{ 1.2f, 1.0f, 2.0f },
		m_Shininess(32.0f),
		m_DiffuseMap("res", "textures/container.png", aiTextureType_DIFFUSE),
		m_SpecularMap("res", "textures/container_specular.png", aiTextureType_SPECULAR),
		m_EmissionMap("res", "textures/emission_map.png", aiTextureType_EMISSIVE),
		m_RotateCube(0),
		m_LightingShader("res/shaders/Lighting/Basic/lighting_maps.vs.glsl", "res/shaders/Lighting/Basic/lighting_maps.fs.glsl"),
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

		m_DiffuseMap.Load(0);
		m_SpecularMap.Load(0);
		m_EmissionMap.Load(0);

		m_LightingShader.use();
		m_LightingShader.setInt("material.diffuse", 0);
		m_LightingShader.setInt("material.specular", 1);
		m_LightingShader.setInt("material.emission", 2);
	}

	TestLightingMaps::~TestLightingMaps()
	{
		GLCall(glDisable(GL_DEPTH_TEST));
		GLCall(glDisable(GL_BLEND));

		GLCall(glDeleteBuffers(1, &m_VBO));
		GLCall(glDeleteVertexArrays(1, &m_CubeVAO));
		GLCall(glDeleteVertexArrays(1, &m_LightCubeVAO));

		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	void TestLightingMaps::OnRender(Camera camera)
	{
		m_Camera = camera;

		if (m_RotateCube)
		{
			m_LightPos.x = 2.0f * sin(glfwGetTime());
			m_LightPos.z = 1.5f * cos(glfwGetTime());
		}

		m_LightingShader.use();
		m_LightingShader.setVec3("light.position", m_LightPos);
		m_LightingShader.setVec3("viewPos", camera.Position);

		// light properties
		m_LightingShader.setVec3("light.ambient", glm::vec3(0.2f));
		m_LightingShader.setVec3("light.diffuse", glm::vec3(0.5f));
		m_LightingShader.setVec3("light.specular", glm::vec3(1.0f));

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
		GLCall(glBindTexture(GL_TEXTURE_2D, m_DiffuseMap.Id));

		// binding specular map
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_SpecularMap.Id));

		GLCall(glActiveTexture(GL_TEXTURE2));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_EmissionMap.Id));

		// render the cube
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

	void TestLightingMaps::OnImGuiRender()
	{
		ImGui::SliderFloat3("Light Position", &m_LightPos.x, -5.0f, 5.0f);
		ImGui::SliderFloat("Shininess", &m_Shininess, 0.0f, 64.0f);
		ImGui::SliderInt("Rotate", &m_RotateCube, 0, 1);
	}
}