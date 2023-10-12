#include "glpch.h"

#include "TestBlinnPhong.h"

namespace test {

	TestBlinnPhong::TestBlinnPhong()
		:
		m_VAO(0),
		m_VBO(0),
		m_FloorTex(0),
		m_LightPos(0.0f),
		m_Blinn(true),
		m_Shininess(32),
		m_Shader("res/shaders/Lighting/Advanced/blinn_phong.vs.glsl", "res/shaders/Lighting/Advanced/blinn_phong.fs.glsl")
	{
		float planeVertices[] = {
			// positions            // normals         // texcoords
			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

			 10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
			 10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

		m_FloorTex = LoadTexture("res/textures/wood.png");

		m_Shader.use();
		m_Shader.setInt("texture1", 0);
	}

	TestBlinnPhong::~TestBlinnPhong()
	{
	}

	void TestBlinnPhong::OnRender(Camera camera)
	{
		m_Shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		m_Shader.setMat4("projection", projection);
		m_Shader.setMat4("view", view);

		m_Shader.setVec3("viewPos", camera.Position);
		m_Shader.setVec3("lightPos", m_LightPos);
		m_Shader.setInt("blinn", m_Blinn);
		m_Shader.setInt("shininess", m_Shininess);

		glBindVertexArray(m_VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_FloorTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		std::cout << (m_Blinn ? "Blinn-Phong" : "Phong") << std::endl;

	}

	void TestBlinnPhong::OnImGuiRender()
	{
		ImGui::DragFloat3("Light Pos", &m_LightPos.x, 1.0f, -10.0f, 10.0f);
		ImGui::SliderInt("Shininess", &m_Shininess, 1, 1024);
		ImGui::Checkbox("Blinn", &m_Blinn);
	}
}