#include "glpch.h"

#include "TestDepth.h"

namespace test {


	TestDepth::TestDepth()
		:
		m_Shader("res/shaders/Advanced/depth_test.vs.glsl", "res/shaders/Advanced/depth_test.fs.glsl"),
		m_Model("res/objects/sponza/sponza.obj")
	{
		GLCall(glEnable(GL_DEPTH_TEST));
		//GLCall(glDepthFunc(GL_ALWAYS)); // always pass the depth test (same effect as glDisable(GL_DEPTH_TEST)

		stbi_set_flip_vertically_on_load(true);
	}

	TestDepth::~TestDepth()
	{
		GLCall(glDepthFunc(GL_LESS));
		m_Model.Destroy();
	}

	void TestDepth::OnRender(Camera camera)
	{
		m_Shader.use();

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		m_Shader.setMat4("view", view);
		m_Shader.setMat4("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		m_Shader.setMat4("model", model);
		m_Model.Draw(m_Shader);

		GLCall(glBindVertexArray(0));
	}

	void TestDepth::OnImGuiRender()
	{

	}
}