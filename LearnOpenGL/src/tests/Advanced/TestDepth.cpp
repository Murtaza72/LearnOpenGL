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
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_ALWAYS)); // always pass the depth test (same effect as GLCall(glDisable(GL_DEPTH_TEST))

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
		GLCall(glGenVertexArrays(1, &m_CubeVAO));
		GLCall(glGenBuffers(1, &m_CubeVBO));
		GLCall(glBindVertexArray(m_CubeVAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_CubeVBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
		GLCall(glBindVertexArray(0));

		// plane VAO
		GLCall(glGenVertexArrays(1, &m_PlaneVAO));
		GLCall(glGenBuffers(1, &m_PlaneVBO));
		GLCall(glBindVertexArray(m_PlaneVAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_PlaneVBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
		GLCall(glBindVertexArray(0));

		m_CubeTexture = LoadTexture("res/textures/marble.jpg");
		m_FloorTexture = LoadTexture("res/textures/metal.png");

		m_Shader.use();
		m_Shader.setInt("texture1", 0);
	}

	TestDepth::~TestDepth()
	{
		GLCall(glDepthFunc(GL_LESS));
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
		GLCall(glBindVertexArray(m_CubeVAO));

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_CubeTexture));
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		m_Shader.setMat4("model", model);

		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		m_Shader.setMat4("model", model);

		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		// floor
		GLCall(glBindVertexArray(m_PlaneVAO));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_FloorTexture));
		m_Shader.setMat4("model", glm::mat4(1.0f));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

		GLCall(glBindVertexArray(0));
	}

	void TestDepth::OnImGuiRender()
	{

	}
}