#include "glpch.h"

#include "Texture.h"

Texture::Texture(std::string m_Name)
	: m_Name(m_Name), m_TexType(aiTextureType_NONE), m_Id(0)
{
}

Texture::Texture(const char* path, aiTextureType texType)
	: m_Path(path), m_TexType(texType), m_Id(0)
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_Id);
}

void Texture::SetParams(GLenum texMinFilter, GLenum texMagFilter, GLenum wrapS, GLenum wrapT)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texMinFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texMagFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
}

void Texture::Load(bool flip)
{
	stbi_set_flip_vertically_on_load(flip);

	glGenTextures(1, &m_Id);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(m_Path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = 0;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, m_Id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else
	{
		std::cout << "Texture failed to load at path: " << m_Path << std::endl;
	}

	stbi_image_free(data);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, m_Id);
}