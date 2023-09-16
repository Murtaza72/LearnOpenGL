#include "TestTriangle.h"

#include "GL/glew.h"

namespace test
{
	TestTriangle::TestTriangle()
		:
		m_Shader{ "res/shaders/shader.vs", "res/shaders/shader.fs" },
		m_VBO(0)

	{
		float vertices[] = {
			// positions         // colors
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
			-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
			 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
		};


		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	TestTriangle::~TestTriangle()
	{

	}

	void TestTriangle::OnRender()
	{
		m_Shader.use();

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	void TestTriangle::OnImGuiRender()
	{

	}
}