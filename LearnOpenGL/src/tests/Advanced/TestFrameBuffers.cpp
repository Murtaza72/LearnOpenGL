#include "glpch.h"

#include "TestFrameBuffers.h"

namespace test {

	TestFrameBuffers::TestFrameBuffers()
		:
		m_FrameBuffer(0),
		m_TextureColorBuffer(0),
		m_RenderBuffer(0),
		m_Model("res/objects/sponza/sponza.obj"),
		m_Shader("res/shaders/Advanced/framebuffer.vs.glsl", "res/shaders/Advanced/framebuffer.fs.glsl"),
		m_ScreenShader("res/shaders/Advanced/framebuffer_screen.vs.glsl", "res/shaders/Advanced/framebuffer_screen.fs.glsl")
	{
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		glGenVertexArrays(1, &m_QuadVAO);
		glGenBuffers(1, &m_QuadVBO);
		glBindVertexArray(m_QuadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_QuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glGenFramebuffers(1, &m_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

		// generate texture
		glGenTextures(1, &m_TextureColorBuffer);
		glBindTexture(GL_TEXTURE_2D, m_TextureColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		// attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureColorBuffer, 0);

		glGenRenderbuffers(1, &m_RenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBuffer);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_WIDTH);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBuffer);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		for (int i = 0; i < 9; i++)
		{
			m_Kernel[i] = 2;
		}
		m_Kernel[4] = -15;
		m_DivFactor = 1;
	}

	TestFrameBuffers::~TestFrameBuffers()
	{
	}

	void TestFrameBuffers::OnRender(Camera camera)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Shader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		m_Shader.setMat4("view", view);
		m_Shader.setMat4("projection", projection);

		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.01f));
		m_Shader.setMat4("model", model);
		m_Model.Draw(m_Shader);

		// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
		// clear all relevant buffers
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
		glClear(GL_COLOR_BUFFER_BIT);

		m_ScreenShader.use();

		for (int i = 0; i < 9; i++)
		{

			m_ScreenShader.setFloat("kernel[" + std::to_string(i) + "]", (m_Kernel[i] / m_DivFactor));
		}

		glBindVertexArray(m_QuadVAO);
		glBindTexture(GL_TEXTURE_2D, m_TextureColorBuffer);	// use the color attachment texture as the texture of the quad plane
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	void TestFrameBuffers::OnImGuiRender()
	{
		ImGui::Begin("Kernel Effects");
		if (ImGui::CollapsingHeader("Kernel Value"))
		{
			ImGui::PushItemWidth(100);

			for (int i = 0; i < 9; i++)
			{
				ImGui::PushID(i);
				if (i % 3 != 0)
					ImGui::SameLine();
				ImGui::DragFloat("", &m_Kernel[i], 1.0f, -20.0f, 20.0f);
				ImGui::PopID();
			}
			ImGui::PopItemWidth();

			ImGui::DragFloat("Div Factor", &m_DivFactor, 1.0f, 1.0f, 20.0f);
		}
		ImGui::End();
	}
}