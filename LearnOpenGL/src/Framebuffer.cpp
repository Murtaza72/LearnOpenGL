#include "glpch.h"

#include "Framebuffer.h"
//#include "Cubemap.h"


Framebuffer::Framebuffer()
	: Id(0), m_Width(0), m_Height(0), m_BitCombo(0)
{
	glGenFramebuffers(1, &Id);
}

Framebuffer::Framebuffer(GLuint width, GLuint height, GLbitfield bitCombo)
	: Id(0), m_Width(width), m_Height(height), m_BitCombo(bitCombo)
{
	glGenFramebuffers(1, &Id);
}

Framebuffer::~Framebuffer()
{
	glDeleteRenderbuffers(m_RBOs.size(), &m_RBOs[0]);
	glDeleteFramebuffers(1, &Id);
}

void Framebuffer::DisableColorBuffer()
{
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, Id);
}

void Framebuffer::SetViewport()
{
	glViewport(0, 0, m_Width, m_Height);
}

void Framebuffer::Clear()
{
	glClear(m_BitCombo);
}

void Framebuffer::Activate()
{
	Bind();
	SetViewport();
	Clear();
}

void Framebuffer::AllocateAndAttachRBO(GLenum attachType, GLenum format)
{
	GLuint rbo;

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);

	glRenderbufferStorage(GL_RENDERBUFFER, format, m_Width, m_Height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachType, GL_RENDERBUFFER, rbo);

	m_RBOs.push_back(rbo);
}

void Framebuffer::AllocateAndAttachTexture(GLenum attachType, GLenum format, GLenum type)
{
	std::string name = "tex" + m_Textures.size();
	Texture texture(name);

	// Allocate
	texture.Bind();
	texture.Allocate(format, m_Width, m_Height, type);
	texture.SetParams(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);

	// sets border color for each border texel
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER, attachType, GL_TEXTURE_2D, texture.Id, 0);

	m_Textures.push_back(texture);
}

void Framebuffer::AttachTexture(GLenum attachType, Texture tex)
{
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachType, GL_TEXTURE_2D, tex.Id, 0);
}

/*void Framebuffer::allocateAndAttachCubemap(GLenum attachType, GLenum format, GLenum type)
{
	cubemap = Cubemap();

	cubemap.generate();
	cubemap.Bind();
	cubemap.Allocate(format, m_Width, m_Height, type);

	glFramebufferTexture(GL_FRAMEBUFFER, attachType, cubemap.id, 0);
}*/
