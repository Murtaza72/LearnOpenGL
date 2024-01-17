#include "glpch.h"

#include "TestDeferredRendering.h"

namespace test {

	TestDeferredRendering::TestDeferredRendering()
		:
		m_GBuffer(SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT),
		m_BackpackModel("res/objects/backpack/backpack.obj"),
		m_GeometryPassShader("res/shaders/Lighting/Advanced/deferred/geometry_pass.vs.glsl", "res/shaders/Lighting/Advanced/deferred/geometry_pass.fs.glsl"),
		m_LightingPassShader("res/shaders/Lighting/Advanced/deferred/lighting_pass.vs.glsl", "res/shaders/Lighting/Advanced/deferred/lighting_pass.fs.glsl"),
		m_LightCubeShader("res/shaders/Lighting/Advanced/deferred/lightcube.vs.glsl", "res/shaders/Lighting/Advanced/deferred/lightcube.fs.glsl")
	{
		GLCall(glEnable(GL_DEPTH_TEST));

		// Position
		m_GBuffer.AllocateAndAttachTexture(GL_COLOR_ATTACHMENT0, GL_RGBA16F, GL_RGBA, GL_FLOAT,
										   GL_NEAREST,
										   GL_NEAREST);

		// Normal
		m_GBuffer.AllocateAndAttachTexture(GL_COLOR_ATTACHMENT1, GL_RGBA16F, GL_RGBA, GL_FLOAT,
										   GL_NEAREST,
										   GL_NEAREST);

		// Albedo + Specular
		m_GBuffer.AllocateAndAttachTexture(GL_COLOR_ATTACHMENT2, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
										   GL_NEAREST,
										   GL_NEAREST);

		unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		GLCall(glDrawBuffers(3, attachments));

		m_GBuffer.AllocateAndAttachRBO(GL_DEPTH_ATTACHMENT, GL_DEPTH_COMPONENT);

		m_GBuffer.IsComplete();
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		m_ModelPositions.push_back(glm::vec3(-3.0, -0.5, -3.0));
		m_ModelPositions.push_back(glm::vec3(0.0, -0.5, -3.0));
		m_ModelPositions.push_back(glm::vec3(3.0, -0.5, -3.0));
		m_ModelPositions.push_back(glm::vec3(-3.0, -0.5, 0.0));
		m_ModelPositions.push_back(glm::vec3(0.0, -0.5, 0.0));
		m_ModelPositions.push_back(glm::vec3(3.0, -0.5, 0.0));
		m_ModelPositions.push_back(glm::vec3(-3.0, -0.5, 3.0));
		m_ModelPositions.push_back(glm::vec3(0.0, -0.5, 3.0));
		m_ModelPositions.push_back(glm::vec3(3.0, -0.5, 3.0));

		const unsigned int NR_LIGHTS = 32;
		srand(13);
		for (unsigned int i = 0; i < NR_LIGHTS; i++)
		{
			// calculate slightly random offsets
			float xPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
			float yPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
			float zPos = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
			m_LightCubes.first.push_back(glm::vec3(xPos, yPos, zPos));
			// also calculate random color
			float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
			float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
			float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
			m_LightCubes.second.push_back(glm::vec3(xPos, yPos, zPos));
		}

		m_LightingPassShader.use();
		m_LightingPassShader.setInt("gPosition", 0);
		m_LightingPassShader.setInt("gNormal", 1);
		m_LightingPassShader.setInt("gAlbedoSpec", 2);
	}

	TestDeferredRendering::~TestDeferredRendering()
	{
		m_GBuffer.Destroy();
		m_BackpackModel.Destroy();
	}

	void TestDeferredRendering::OnRender(Camera camera)
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_GBuffer.Activate();

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);

		m_GeometryPassShader.use();
		m_GeometryPassShader.setMat4("projection", projection);
		m_GeometryPassShader.setMat4("view", view);

		for (unsigned int i = 0; i < m_ModelPositions.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, m_ModelPositions[i]);
			model = glm::scale(model, glm::vec3(0.5f));
			m_GeometryPassShader.setMat4("model", model);
			m_BackpackModel.Draw(m_GeometryPassShader);
		}
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		// 2. lighting pass: calculate lighting by iterating over a screen filled quad pixel-by-pixel using the gbuffer's content.
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		m_LightingPassShader.use();
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_GBuffer.GetTexture(0)));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_GBuffer.GetTexture(1)));
		GLCall(glActiveTexture(GL_TEXTURE2));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_GBuffer.GetTexture(2)));

		for (unsigned int i = 0; i < m_LightCubes.first.size(); i++)
		{
			m_LightingPassShader.setVec3("lights[" + std::to_string(i) + "].Position", m_LightCubes.first[i]);
			m_LightingPassShader.setVec3("lights[" + std::to_string(i) + "].Color", m_LightCubes.second[i]);

			const float constant = 1.0f;
			const float linear = 0.7f;
			const float quadratic = 1.8f;
			m_LightingPassShader.setFloat("lights[" + std::to_string(i) + "].Linear", linear);
			m_LightingPassShader.setFloat("lights[" + std::to_string(i) + "].Quadratic", quadratic);

			float lightMax = std::fmaxf(std::fmaxf(m_LightCubes.second[i].r, m_LightCubes.second[i].g), m_LightCubes.second[i].b);
			float radius =
				(-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax)))
				/ (2 * quadratic);

			m_LightingPassShader.setFloat("lights[" + std::to_string(i) + "].Radius", radius);
		}
		m_LightingPassShader.setVec3("viewPos", camera.Position);
		RenderQuad();

		// 2.5. copy content of geometry's depth buffer to default framebuffer's depth buffer
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer.Id));
		GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
		GLCall(glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

		// 3. render lights on top of scene
		m_LightCubeShader.use();
		m_LightCubeShader.setMat4("projection", projection);
		m_LightCubeShader.setMat4("view", view);
		for (unsigned int i = 0; i < m_LightCubes.first.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, m_LightCubes.first[i]);
			model = glm::scale(model, glm::vec3(0.125f));
			m_LightCubeShader.setMat4("model", model);
			m_LightCubeShader.setVec3("lightColor", m_LightCubes.second[i]);
			RenderCube();
		}
	}

	void TestDeferredRendering::OnImGuiRender()
	{

	}
}