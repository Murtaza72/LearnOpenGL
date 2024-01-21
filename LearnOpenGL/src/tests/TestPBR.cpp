#include "glpch.h"

#include "TestPBR.h"

namespace test {

	TestPBR::TestPBR()
		:
		m_Shader("res/shaders/PBR/pbr.vs.glsl", "res/shaders/PBR/pbr.fs.glsl"),
		m_Albedo("res", "textures/pbr/albedo.png", aiTextureType_DIFFUSE),
		m_Normal("res", "textures/pbr/normal.png", aiTextureType_NORMALS),
		m_Metallic("res", "textures/pbr/metallic.png", aiTextureType_METALNESS),
		m_Rough("res", "textures/pbr/roughness.png", aiTextureType_DIFFUSE_ROUGHNESS),
		m_AO("res", "textures/pbr/ao.png", aiTextureType_AMBIENT_OCCLUSION)
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		m_Lights.first.push_back(glm::vec3(0.0f, 0.0f, 10.0f));
		m_Lights.second.push_back(glm::vec3(150.0f, 150.0f, 150.0f));

		m_Albedo.Load(0);
		m_Normal.Load(0);
		m_Metallic.Load(0);
		m_Rough.Load(0);

		m_Shader.use();
		m_Shader.setInt("albedoMap", 0);
		m_Shader.setInt("normalMap", 1);
		m_Shader.setInt("metallicMap", 2);
		m_Shader.setInt("roughnessMap", 3);
		m_Shader.setInt("aoMap", 4);
	}

	TestPBR::~TestPBR()
	{
		m_Albedo.Destroy();
		m_Normal.Destroy();
		m_Metallic.Destroy();
		m_Rough.Destroy();
		m_AO.Destroy();
	}

	void TestPBR::OnRender(Camera camera)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();

		m_Shader.use();
		m_Shader.setMat4("projection", projection);
		m_Shader.setMat4("view", view);
		m_Shader.setVec3("camPos", camera.Position);

		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Albedo.Id));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Normal.Id));
		GLCall(glActiveTexture(GL_TEXTURE2));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Metallic.Id));
		GLCall(glActiveTexture(GL_TEXTURE3));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Rough.Id));
		GLCall(glActiveTexture(GL_TEXTURE4));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_AO.Id));

		// render rows*column number of spheres with material properties defined by textures (they all have the same material properties)
		glm::mat4 model = glm::mat4(1.0f);
		for (int row = 0; row < nrRows; ++row)
		{
			for (int col = 0; col < nrColumns; ++col)
			{
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(
					(float)(col - (nrColumns / 2)) * spacing,
					(float)(row - (nrRows / 2)) * spacing,
					0.0f
				));
				m_Shader.setMat4("model", model);
				m_Shader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
				RenderSphere();
			}
		}

		// render light source (simply re-render sphere at light positions)
		// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
		// keeps the codeprint small.
		for (unsigned int i = 0; i < m_Lights.first.size(); ++i)
		{
			glm::vec3 newPos = m_Lights.first[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = m_Lights.first[i];
			m_Shader.setVec3("lightPositions[" + std::to_string(i) + "]", newPos);
			m_Shader.setVec3("lightColors[" + std::to_string(i) + "]", m_Lights.second[i]);

			model = glm::mat4(1.0f);
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			m_Shader.setMat4("model", model);
			m_Shader.setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
			RenderSphere();
		}
	}

	void TestPBR::OnImGuiRender()
	{

	}
}