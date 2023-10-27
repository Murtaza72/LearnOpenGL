#include "glpch.h"

#include "TestShadowMapping.h"

namespace test {

	TestShadowMapping::TestShadowMapping()
		:
		m_DepthMap(0),
		m_DepthMapFBO(0),
		m_LightPos({ -2.0f, 4.0f, -1.0f }),
		m_WoodTexture(0),
		m_Shader("res/shaders/Lighting/Advanced/shadow mapping/shadow.vs.glsl", "res/shaders/Lighting/Advanced/shadow mapping/shadow.fs.glsl"),
		m_DepthShader("res/shaders/Lighting/Advanced/shadow mapping/depth.vs.glsl", "res/shaders/Lighting/Advanced/shadow mapping/depth.fs.glsl")
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		m_WoodTexture = LoadTexture("res/textures/wood.png");

		GLCall(glGenFramebuffers(1, &m_DepthMapFBO));

		GLCall(glGenTextures(1, &m_DepthMap));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthMap));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
							SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0));
		GLCall(glDrawBuffer(GL_NONE));
		GLCall(glReadBuffer(GL_NONE));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		m_Shader.use();
		m_Shader.setInt("diffuseTexture", 0);
		m_Shader.setInt("shadowMap", 1);

	}

	TestShadowMapping::~TestShadowMapping()
	{
	}

	void TestShadowMapping::OnRender(Camera camera)
	{
		// 1. first render to depth map
		GLCall(glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO));
		GLCall(glClear(GL_DEPTH_BUFFER_BIT));

		float near_plane = 1.0f, far_plane = 7.5f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

		glm::mat4 lightView = glm::lookAt(m_LightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f));
		model = glm::scale(model, glm::vec3(0.01f));

		glm::mat4 lightSpaceMatrix = lightProjection * lightView;
		m_DepthShader.use();
		m_DepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		m_DepthShader.setMat4("model", model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_WoodTexture);

		RenderScene(m_DepthShader);

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		// 2. then render scene as normal with shadow mapping (using depth map)
		GLCall(glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		m_Shader.use();
		m_Shader.setMat4("view", view);
		m_Shader.setMat4("projection", projection);
		m_Shader.setMat4("model", model);
		m_Shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		m_Shader.setVec3("viewPos", camera.Position);
		m_Shader.setVec3("lightPos", m_LightPos);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_WoodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_DepthMap);
		RenderScene(m_Shader);
	}

	void TestShadowMapping::OnImGuiRender()
	{
		ImGui::DragFloat3("Light Pos", &m_LightPos.x, 1.0f, -10.0, 10.0f);
	}
}

