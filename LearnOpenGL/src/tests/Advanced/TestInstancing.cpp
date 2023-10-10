#include "glpch.h"

#include "TestInstancing.h"

namespace test {

	TestInstancing::TestInstancing()
		:
		m_VAO(0),
		m_VBO(0),
		m_InstanceVBO(0),
		m_Shader("res/shaders/Advanced/instancing.vs.glsl", "res/shaders/Advanced/instancing.fs.glsl")
	{
		float quadVertices[] = {
			// positions     // colors
			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

			-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
			 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
			 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
		};

		GLCall(glGenVertexArrays(1, &m_VAO));
		GLCall(glBindVertexArray(m_VAO));

		GLCall(glGenBuffers(1, &m_VBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float))));

		glm::vec2 translations[100];
		int index = 0;
		float offset = 0.1f;
		for (int y = -10; y < 10; y += 2)
		{
			for (int x = -10; x < 10; x += 2)
			{
				glm::vec2 translation;
				translation.x = (float)x / 10.0f + offset;
				translation.y = (float)y / 10.0f + offset;
				translations[index++] = translation;
			}
		}

		GLCall(glGenBuffers(1, &m_InstanceVBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_InstanceVBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, translations, GL_STATIC_DRAW));

		GLCall(glEnableVertexAttribArray(2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0));
		GLCall(glVertexAttribDivisor(2, 1));
	}

	TestInstancing::~TestInstancing()
	{
	}

	void TestInstancing::OnRender(Camera camera)
	{
		m_Shader.use();
		GLCall(glBindVertexArray(m_VAO));
		GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100));
	}

	void TestInstancing::OnImGuiRender()
	{
	}
}