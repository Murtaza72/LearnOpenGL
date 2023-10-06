#include "TestModel.h"

namespace test {

	TestModel::TestModel()
		:
		m_Shader("res/shaders/model/model_loading.vs.glsl", "res/shaders/model/model_loading.fs.glsl"),
		m_Model("res/objects/backpack/backpack.obj")
	{
		// move the assimp-vc143-mt.dll to the x64/Debug folder 
		stbi_set_flip_vertically_on_load(true);

		glEnable(GL_DEPTH_TEST);

		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	TestModel::~TestModel()
	{

	}

	void TestModel::OnRender(Camera camera)
	{
		m_Shader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		m_Shader.setMat4("projection", projection);
		m_Shader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		m_Shader.setMat4("model", model);
		m_Model.Draw(m_Shader);
	}

	void TestModel::OnImGuiRender()
	{

	}
}
