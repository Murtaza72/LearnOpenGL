#include "TestDepth.h"

namespace test {


	TestDepth::TestDepth()
		:
		m_CubeVAO(0),
		m_CubeVBO(0),
		m_PlaneVAO(0),
		m_PlaneVBO(0),
		m_CubeTexture(0),
		m_FloorTexture(0),
		m_Shader("res/shaders/Advanced/depth_test.vs.glsl", "res/shaders/Advanced/depth_test.fs.glsl")
	{
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_ALWAYS); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST))

		float cubeVertices[] = {
			// positions          // texture Coords
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		float planeVertices[] = {
			// positions          // texture Coords
			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
			 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
		};

		// cube VAO
		glGenVertexArrays(1, &m_CubeVAO);
		glGenBuffers(1, &m_CubeVBO);
		glBindVertexArray(m_CubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		// plane VAO
		glGenVertexArrays(1, &m_PlaneVAO);
		glGenBuffers(1, &m_PlaneVBO);
		glBindVertexArray(m_PlaneVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_PlaneVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glBindVertexArray(0);

		m_CubeTexture = LoadTexture("res/textures/marble.jpg");
		m_FloorTexture = LoadTexture("res/textures/metal.png");

		m_Shader.use();
		m_Shader.setInt("texture1", 0);
	}

	TestDepth::~TestDepth()
	{

	}

	void TestDepth::OnRender(Camera camera)
	{
		m_Shader.use();

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		m_Shader.setMat4("view", view);
		m_Shader.setMat4("projection", projection);

		// cubes
		glBindVertexArray(m_CubeVAO);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_CubeTexture);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		m_Shader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		m_Shader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		// floor
		glBindVertexArray(m_PlaneVAO);
		glBindTexture(GL_TEXTURE_2D, m_FloorTexture);
		m_Shader.setMat4("model", glm::mat4(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

	}

	void TestDepth::OnImGuiRender()
	{

	}
}
