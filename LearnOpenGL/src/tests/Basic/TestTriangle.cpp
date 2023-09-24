#include "TestTriangle.h"

namespace test {

	TestTriangle::TestTriangle()
		:
		m_Shader{ "res/shaders/Basic/shader.vs.glsl", "res/shaders/Basic/shader.fs.glsl" },
		m_VBO(0),
		m_VAO(0)
	{
		float vertices[] = {
			// positions         // colors
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
		};

		GLCall(glGenBuffers(1, &m_VBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

		// need VertexArray in core profile otherwise OpenGL won't render
		GLCall(glGenVertexArrays(1, &m_VAO));
		GLCall(glBindVertexArray(m_VAO));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0));

		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
	}

	TestTriangle::~TestTriangle()
	{
		GLCall(glDeleteBuffers(1, &m_VBO));
	}

	void TestTriangle::OnRender(Camera camera)
	{
		m_Shader.use();

		GLCall(glBindVertexArray(m_VAO));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
	}

	void TestTriangle::OnImGuiRender()
	{
	}
}