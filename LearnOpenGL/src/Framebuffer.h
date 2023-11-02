#pragma once

#include "Texture.h"

class Framebuffer
{
public:
	GLuint Id;

public:
	Framebuffer();
	Framebuffer(GLuint width, GLuint height, GLbitfield bitCombo);
	void Destroy();

	void DisableColorBuffer();
	void Bind();
	void SetViewport();
	void Clear();
	void Activate();
	void ActivateTexture();

	void AllocateAndAttachRBO(GLenum attachmentType, GLenum format);
	void AllocateAndAttachTexture(GLenum attachmentType, GLenum internalFormat,
								  GLenum format, GLenum type,
								  GLint texMinFilter = GL_NEAREST,
								  GLint texMagFilter = GL_NEAREST,
								  GLint wrapS = GL_REPEAT,
								  GLint wrapT = GL_REPEAT);

	void AttachTexture(Texture tex);

private:
	GLuint m_Width;
	GLuint m_Height;
	GLbitfield m_BitCombo;

	std::vector<GLuint> m_RBOs;
	std::vector<Texture> m_Textures;
	//Cubemap cubemap;
};