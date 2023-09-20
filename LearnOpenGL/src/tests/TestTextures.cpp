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
		m_Shader("res/shaders/texture.vs", "res/shaders/texture.fs")
	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

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

		GLCall(glGenVertexArrays(1, &m_VAO));
		GLCall(glBindVertexArray(m_VAO));

		GLCall(glGenBuffers(1, &m_VBO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

		GLCall(glGenBuffers(1, &m_IBO));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));

		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));

		m_TextureID = LoadTexture("res/textures/me.png");

		m_Shader.use();
		m_Shader.setInt("tex1", 0);
	}

	TestTextures::~TestTextures()
	{
		GLCall(glDeleteVertexArrays(1, &m_VAO));
		GLCall(glDeleteBuffers(1, &m_VBO));
		GLCall(glDeleteVertexArrays(1, &m_IBO));

		GLCall(glBindTexture(GL_TEXTURE_2D, 0))
	}

	void TestTextures::OnRender(Camera camera)
	{
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_TextureID));

		GLCall(glBindVertexArray(m_VAO));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
	}

	void TestTextures::OnImGuiRender()
	{
	}

	unsigned int TestTextures::LoadTexture(const std::string& path)
	{
		unsigned int textureID;
		GLCall(GLCall(glGenTextures(1, &textureID)));

		stbi_set_flip_vertically_on_load(true);

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
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
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