#pragma once

#include <assimp/scene.h>

class Texture
{
public:
	unsigned int m_Id;

public:
	Texture(std::string name);
	Texture(const char* path, aiTextureType type);
	~Texture();

	void Load(bool flip = true);

	void SetParams(GLenum texMinFilter = GL_NEAREST,
				   GLenum texMagFilter = GL_NEAREST,
				   GLenum wrapS = GL_REPEAT,
				   GLenum wrapT = GL_REPEAT);

	void Bind();

private:
	// texture type
	aiTextureType m_TexType;
	std::string m_Name;
	const char* m_Path;
};