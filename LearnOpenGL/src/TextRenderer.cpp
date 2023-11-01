#include "glpch.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "TextRenderer.h"
#include "Utils.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

TextRenderer::TextRenderer(std::string fontPath, unsigned int fontSize)
	:
	m_VAO(0),
	m_VBO(0),
	m_Transfrom(glm::mat4(1.0f)),
	m_Shader("res/shaders/Text/text_rendering.vs.glsl", "res/shaders/Text/text_rendering.fs.glsl")
{
	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT);
	m_Shader.use();
	m_Shader.setMat4("projection", projection);

	float vertex[] = {
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f
	};

	GLCall(glGenVertexArrays(1, &m_VAO));
	GLCall(glBindVertexArray(m_VAO));

	GLCall(glGenBuffers(1, &m_VBO));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	Load(fontPath, fontSize);
}

TextRenderer::~TextRenderer()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));
}

void TextRenderer::Load(std::string fontPath, unsigned int fontSize)
{
	m_Characters.clear();

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	}

	FT_Face face;
	if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, fontSize);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
	}

	GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // disable byte-alignment restriction

	for (unsigned char c = 0; c < 128; c++)
	{
		// load character GLCall(glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		// generate texture
		unsigned int texture;
		GLCall(glGenTextures(1, &texture));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture));
		GLCall(glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		));

		// set texture options
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

		// now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		m_Characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
{
	m_Shader.use();
	m_Shader.setVec3("textColor", color);
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindVertexArray(m_VAO));

	float copyX = x;

	// iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = m_Characters[*c];

		if (*c == '\n')
		{
			y -= ((ch.size.y)) * 1.3 * scale;
			x = copyX;
		}
		else if (*c == ' ')
		{
			x += (ch.advance / 64) * scale;
		}
		else
		{
			float xpos = x + ch.bearing.x * scale;
			float ypos = y - (ch.size.y - ch.bearing.y) * scale;

			m_Transfrom = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(ch.size.x * scale, ch.size.y * scale, 0.0f));
			m_Shader.setMat4("transform", m_Transfrom);

			// render GLCall(glyph texture over quad
			GLCall(glBindTexture(GL_TEXTURE_2D, ch.textureID));

			// update content of VBO memory
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));

			// render quad
			GLCall(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));
			// now advance cursors for next GLCall(glyph (note that advance is number of 1/64 pixels)

			x += (ch.advance / 64) * scale;
		}
	}

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindVertexArray(0));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}