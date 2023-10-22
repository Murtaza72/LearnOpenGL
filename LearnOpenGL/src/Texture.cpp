#include "glpch.h"

#include "Texture.h"
#include "Utils.h"

Texture::Texture(std::string name)
	: name(name), texType(aiTextureType_NONE), Id(0)
{
	GLCall(glGenTextures(1, &Id));
}

Texture::Texture(std::string dir, std::string path, aiTextureType texType)
	: directory(dir), path(path), texType(texType), Id(0)
{
	GLCall(glGenTextures(1, &Id));
}

void Texture::Destroy()
{
	GLCall(glDeleteTextures(1, &Id));
}

// used with textures for framebuffers
void Texture::Allocate(GLenum format, GLuint width, GLuint height, GLenum type)
{
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, nullptr));
}

void Texture::SetParams(GLenum texMinFilter, GLenum texMagFilter, GLenum wrapS, GLenum wrapT)
{
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texMinFilter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texMagFilter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT));
}

void Texture::Load(bool flip)
{
	stbi_set_flip_vertically_on_load(flip);

	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = 0;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		GLCall(glBindTexture(GL_TEXTURE_2D, Id));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}

	stbi_image_free(data);
}

void Texture::Bind()
{
	GLCall(glBindTexture(GL_TEXTURE_2D, Id));
}