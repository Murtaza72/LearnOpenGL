#include "glpch.h"

#include "TestCubemap.h"

namespace test {

	TestCubemap::TestCubemap()
		:
		m_CubemapTex(0),
		m_SkyboxVAO(0),
		m_SkyboxVBO(0),
		m_Faces{ "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" },
		m_Shader("res/shaders/Advanced/cubemap.vs.glsl", "res/shaders/Advanced/cubemap.fs.glsl"),
		m_SkyboxShader("res/shaders/Advanced/skybox.vs.glsl", "res/shaders/Advanced/skybox.fs.glsl"),
		m_Model("res/objects/backpack/backpack.obj")
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		GLCall(glGenVertexArrays(1, &m_SkyboxVAO));
		GLCall(glBindVertexArray(m_SkyboxVAO));

		GLCall(glGenBuffers(1, &m_SkyboxVBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_SkyboxVBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));

		m_CubemapTex = LoadCubeMap(m_Faces, "res/textures/skybox/");

		m_Shader.use();
		m_Shader.setInt("texture1", 0);

		m_SkyboxShader.use();
		m_SkyboxShader.setInt("skybox", 0);
	}

	TestCubemap::~TestCubemap()
	{

	}

	void TestCubemap::OnRender(Camera camera)
	{
		m_Camera = camera;

		m_Shader.use();

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		m_Shader.setMat4("view", view);
		m_Shader.setMat4("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f));
		m_Shader.setMat4("model", model);
		m_Shader.setVec3("cameraPos", camera.Position);
		m_Model.Draw(m_Shader);

		GLCall(glDepthFunc(GL_LEQUAL));
		m_SkyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		m_SkyboxShader.setMat4("view", view);
		m_SkyboxShader.setMat4("projection", projection);

		GLCall(glBindVertexArray(m_SkyboxVAO));
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTex));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		GLCall(glBindVertexArray(0));
		GLCall(glDepthFunc(GL_LESS));
	}

	void TestCubemap::OnImGuiRender()
	{
		ImGui::DragFloat3("Cam Pos", &m_Camera.Position.x, 1.0f, -100.0f, 100.0f);
		ImGui::DragFloat("Yaw", &m_Camera.Yaw, 1.0f, -100.0f, 100.0f);
		ImGui::DragFloat("Pitch", &m_Camera.Pitch, 1.0f, -100.0f, 100.0f);
	}
}