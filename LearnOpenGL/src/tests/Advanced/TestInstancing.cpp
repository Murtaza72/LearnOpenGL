#include "glpch.h"

#include "TestInstancing.h"

namespace test {

	TestInstancing::TestInstancing()
		:
		m_Shader("res/shaders/Advanced/instancing.vs.glsl", "res/shaders/Advanced/instancing.fs.glsl"),
		m_PlanetModel("res/objects/asteroid/planet.obj"),
		m_AsteroidModel("res/objects/asteroid/rock.obj"),
		m_ModelMatrices(0),
		m_Amount(1000)
	{
		glEnable(GL_DEPTH_TEST);

		m_ModelMatrices = new glm::mat4[m_Amount];
		srand(glfwGetTime()); // initialize random seed	
		float radius = 50.0;
		float offset = 2.5f;

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
	}

	TestInstancing::~TestInstancing()
	{
	}

	void TestInstancing::OnRender(Camera camera)
	{
		m_Shader.use();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();;
		m_Shader.setMat4("projection", projection);
		m_Shader.setMat4("view", view);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, -30.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		m_Shader.setMat4("model", model);
		m_PlanetModel.Draw(m_Shader);

		for (int i = 0; i < m_Amount; i++)
		{
			m_Shader.setMat4("model", m_ModelMatrices[i]);
			m_AsteroidModel.Draw(m_Shader);
		}
	}

	void TestInstancing::OnImGuiRender()
	{
	}
}