#include "TestTextures.h"

#include <iostream>

#include "stb_image/stb_image.h"

namespace test
{
	TestTextures::TestTextures()
		:
		m_VBO(0),
		m_VAO(0),
		m_IBO(0),
		m_TextureID(0),
		m_ImageBuffer(nullptr),
		m_Shader("res/shaders/texture.vs", "res/shaders/texture.fs")
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		float vertices[] = {
			// positions       // texture coords
			0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		   -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		   -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
		};

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &m_IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glGenBuffers(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

		m_TextureID = LoadTexture("res/textures/me.png");
	}

	TestTextures::~TestTextures()
	{
	}

	void TestTextures::OnRender(Camera camera)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		m_Shader.use();
		m_Shader.setInt("tex1", 0);

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void TestTextures::OnImGuiRender()
	{

	}

	unsigned int TestTextures::LoadTexture(const std::string& path)
	{
		unsigned int textureID;
		GLCall(glGenTextures(1, &textureID));

		int width, height, nComponents;
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &nComponents, 0);
		if (data)
		{
			GLenum format;
			if (nComponents == 1)
				format = GL_RED;
			else if (nComponents == 3)
				format = GL_RGB;
			else if (nComponents == 4)
				format = GL_RGBA;

			GLCall(glBindTexture(GL_TEXTURE_2D, textureID));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
			GLCall(glGenerateMipmap(GL_TEXTURE_2D));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;
	}
}