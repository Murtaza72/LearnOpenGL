#include "glpch.h"

#include "TestInstancing.h"

namespace test {

	TestInstancing::TestInstancing()
		:
		m_AsteroidShader("res/shaders/Advanced/instancing.vs.glsl", "res/shaders/Advanced/instancing.fs.glsl"),
		m_PlanetShader("res/shaders/Advanced/planet.vs.glsl", "res/shaders/Advanced/planet.fs.glsl"),
		m_PlanetModel("res/objects/asteroid/planet.obj"),
		m_AsteroidModel("res/objects/asteroid/rock.obj"),
		m_ModelMatrices(0),
		m_Amount(10000)
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		m_ModelMatrices = new glm::mat4[m_Amount];
		srand(glfwGetTime()); // initialize random seed	
		float radius = 150.0 / 2;
		float offset = 25.0f / 2;

		for (int i = 0; i < m_Amount; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			// 1. translation: displace along circle with 'radius' in range [-offset, offset]
			float angle = (float)i / (float)m_Amount * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius + displacement;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float z = cos(angle) * radius + displacement;
			model = glm::translate(model, glm::vec3(x, y, z));

			// 2. scale: scale between 0.05 and 0.25f
			float scale = (rand() % 20) / 100.0f + 0.05;
			model = glm::scale(model, glm::vec3(scale));

			// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
			float rotAngle = (rand() % 360);
			model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

			// 4. now add to list of matrices
			m_ModelMatrices[i] = model;
		}

		unsigned int buffer;
		GLCall(glGenBuffers(1, &buffer));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_Amount * sizeof(glm::mat4), m_ModelMatrices, GL_STATIC_DRAW));

		for (unsigned int i = 0; i < m_AsteroidModel.meshes.size(); i++)
		{
			unsigned int VAO = m_AsteroidModel.meshes[i].VAO;
			GLCall(glBindVertexArray(VAO));
			// vertex attributes
			std::size_t vec4Size = sizeof(glm::vec4);

			GLCall(glEnableVertexAttribArray(3));
			GLCall(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0));
			GLCall(glEnableVertexAttribArray(4));
			GLCall(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size)));
			GLCall(glEnableVertexAttribArray(5));
			GLCall(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size)));
			GLCall(glEnableVertexAttribArray(6));
			GLCall(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size)));

			GLCall(glVertexAttribDivisor(3, 1));
			GLCall(glVertexAttribDivisor(4, 1));
			GLCall(glVertexAttribDivisor(5, 1));
			GLCall(glVertexAttribDivisor(6, 1));

			GLCall(glBindVertexArray(0));
		}
	}

	TestInstancing::~TestInstancing()
	{
	}

	void TestInstancing::OnRender(Camera camera)
	{
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();

		m_PlanetShader.use();
		m_PlanetShader.setMat4("projection", projection);
		m_PlanetShader.setMat4("view", view);

		m_AsteroidShader.use();
		m_AsteroidShader.setMat4("projection", projection);
		m_AsteroidShader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, -30.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		m_PlanetShader.use();
		m_PlanetShader.setMat4("model", model);
		m_PlanetModel.Draw(m_PlanetShader);

		m_AsteroidShader.use();
		for (unsigned int i = 0; i < m_AsteroidModel.meshes.size(); i++)
		{
			glBindVertexArray(m_AsteroidModel.meshes[i].VAO);
			glDrawElementsInstanced(
				GL_TRIANGLES, m_AsteroidModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, m_Amount
			);
		}
	}

	void TestInstancing::OnImGuiRender()
	{
	}
}