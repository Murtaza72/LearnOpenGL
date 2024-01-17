#include "glpch.h"

#include "Framebuffer.h"
//#include "Cubemap.h"

#include "Utils.h"

Framebuffer::Framebuffer()
	: Id(0), m_Width(0), m_Height(0), m_BitCombo(0)
{
	GLCall(glGenFramebuffers(1, &Id));
}

Framebuffer::Framebuffer(GLuint width, GLuint height, GLbitfield bitCombo)
	: Id(0), m_Width(width), m_Height(height), m_BitCombo(bitCombo)
{
	GLCall(glGenFramebuffers(1, &Id));
}

void Framebuffer::Destroy()
{
	GLCall(glDeleteFramebuffers(1, &Id));

	if (m_RBOs.size() > 0)
	{
		GLCall(glDeleteRenderbuffers(m_RBOs.size(), &m_RBOs[0]));
	}

	for (int i = 0, len = m_Textures.size(); i < len; i++)
	{
		m_Textures[i].Destroy();
	}
}

void Framebuffer::DisableColorBuffer()
{
	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));
}

void Framebuffer::Bind()
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, Id));
}

void Framebuffer::SetViewport()
{
	GLCall(glViewport(0, 0, m_Width, m_Height));
}

void Framebuffer::Clear()
{
	GLCall(glClear(m_BitCombo));
}

void Framebuffer::Activate()
{
	Bind();
	SetViewport();
	Clear();
}

void Framebuffer::AllocateAndAttachRBO(GLenum attachmentType, GLenum format)
{
	GLuint rbo;

	GLCall(glGenRenderbuffers(1, &rbo));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo));

	Bind();
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, format, m_Width, m_Height));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, rbo));

	m_RBOs.push_back(rbo);
}

void Framebuffer::AllocateAndAttachTexture(GLenum attachmentType, GLenum internalFormat, GLenum format, GLenum type, GLint texMinFilter, GLint texMagFilter, GLint wrapT, GLint wrapS)
{
	std::string name = "tex" + std::to_string(m_Textures.size());
	Texture texture(name);

	// Allocate Texture
	texture.Bind();
	texture.Allocate(internalFormat, format, m_Width, m_Height, type);
	texture.SetParams(texMinFilter, texMagFilter, wrapS, wrapT);

	Bind();
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, GL_TEXTURE_2D, texture.Id, 0));

	m_Textures.push_back(texture);
}

void Framebuffer::AttachTexture(Texture tex)
{
	m_Textures.push_back(tex);
}

void Framebuffer::ActivateTexture()
{
	for (int i = 0, len = m_Textures.size(); i < len; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_Textures[i].Id);
	}
}

unsigned int Framebuffer::GetTexture(int index)
{
	if (index < m_Textures.size())
		return m_Textures[index].Id;
}

bool Framebuffer::IsComplete()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer not complete!" << std::endl;
		__debugbreak();
		return false;
	}
	return true;
}

/*void Framebuffer::allocateAndAttachCubemap(GLenum attachType, GLenum format, GLenum type)
{
	cubemap = Cubemap());

	cubemap.generate());
	cubemap.Bind());
	cubemap.Allocate(format, m_Width, m_Height, type));

	GLCall(glFramebufferTexture(GL_FRAMEBUFFER, attachType, cubemap.id, 0));
}*/
