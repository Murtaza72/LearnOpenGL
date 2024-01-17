#pragma once

#include <assimp/scene.h>

class Texture
{
public:
	unsigned int Id;
	aiTextureType texType;
	std::string name;
	std::string directory;  // required in model
	std::string path;

public:
	Texture(std::string name);
	Texture(std::string dir, std::string path, aiTextureType texType);
	void Destroy();

	void Allocate(GLenum internalFormat, GLenum format, GLuint width, GLuint height, GLenum type, const void* data = nullptr);
	void Load(GLint internalFormat, bool flip = true);
	void SetParams(GLint texMinFilter, GLint texMagFilter, GLint wrapS, GLint wrapT);

	void Bind();
};