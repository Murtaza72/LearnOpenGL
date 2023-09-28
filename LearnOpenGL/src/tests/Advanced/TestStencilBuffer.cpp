#include "TestStencilBuffer.h"

namespace test {

	TestStencilBuffer::TestStencilBuffer()
		:
		m_CubeVAO(0),
		m_CubeVBO(0),
		m_PlaneVAO(0),
		m_PlaneVBO(0),
		m_CubeTexture(0),
		m_FloorTexture(0),
		m_Scale(1.1f),
		m_Shader("res/shaders/Advanced/stencil_test.vs.glsl", "res/shaders/Advanced/stencil_test.fs.glsl"),
		m_OutlineShader("res/shaders/Advanced/stencil_test.vs.glsl", "res/shaders/Advanced/outline_color.fs.glsl")
	{

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glDepthFunc(GL_LESS));

		GLCall(glEnable(GL_STENCIL_TEST));
		GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
		GLCall(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));

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

	TestStencilBuffer::~TestStencilBuffer()
	{
		GLCall(glDeleteVertexArrays(1, &m_CubeVAO));
		GLCall(glDeleteVertexArrays(1, &m_PlaneVAO));
		GLCall(glDeleteBuffers(1, &m_CubeVBO));
		GLCall(glDeleteBuffers(1, &m_PlaneVBO));
		GLCall(glDisable(GL_STENCIL_TEST));
	}

	void TestStencilBuffer::OnRender(Camera camera)
	{
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

		m_OutlineShader.use();
		m_OutlineShader.setMat4("view", view);
		m_OutlineShader.setMat4("projection", projection);

		m_Shader.use();
		m_Shader.setMat4("view", view);
		m_Shader.setMat4("projection", projection);

		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE));

		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

		GLCall(glStencilMask(0x00)); // make sure we don't update the stencil buffer while drawing the floor
		m_Shader.use();
		// floor
		GLCall(glBindVertexArray(m_PlaneVAO));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_FloorTexture));
		m_Shader.setMat4("model", glm::mat4(1.0f));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 6));

		GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
		GLCall(glStencilMask(0xFF));
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

		GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF));
		GLCall(glStencilMask(0x00));
		GLCall(glDisable(GL_DEPTH_TEST));
		m_OutlineShader.use();

		// upscaled cubes
		GLCall(glBindVertexArray(m_CubeVAO));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_CubeTexture));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(m_Scale));
		m_Shader.setMat4("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(m_Scale));
		m_Shader.setMat4("model", model);
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

		GLCall(glStencilMask(0xFF));
		GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF));
		GLCall(glEnable(GL_DEPTH_TEST));

		GLCall(glBindVertexArray(0));
	}

	void TestStencilBuffer::OnImGuiRender()
	{
		ImGui::DragFloat("Outline Scale", &m_Scale, 0.1f, 1.0f, 3.0f);
	}
}