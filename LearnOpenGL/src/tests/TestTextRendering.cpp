#include "glpch.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "TestTextRendering.h"

namespace test {

	TestTextRendering::TestTextRendering()
		:
		m_VAO(0),
		m_VBO(0),
		m_Transfrom(glm::mat4(1.0f)),
		m_Shader("res/shaders/Text/text_rendering.vs.glsl", "res/shaders/Text/text_rendering.fs.glsl")
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		FT_Library ft;

		if (FT_Init_FreeType(&ft))
		{
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		}

		FT_Face face;
		if (FT_New_Face(ft, "res/fonts/ARIAL.TTF", 0, &face))
		{
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		}

		FT_Set_Pixel_Sizes(face, 0, 48);

		if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		}

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

		for (unsigned char c = 0; c < 128; c++)
		{
			// load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}

			// generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);

			// set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

		float vertex[] = {
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

		for (int i = 0; i < 30; i++)
		{
			m_Text += "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed vel neque in arcu laoreet ultricies.\n";
		}
	}

	TestTextRendering::~TestTextRendering()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void TestTextRendering::OnRender(Camera camera)
	{
		glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT);
		m_Shader.use();
		m_Shader.setMat4("projection", projection);

		RenderText(m_Shader, m_Text, 0.0, (float)SCREEN_HEIGHT - 30.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
	}

	void TestTextRendering::OnImGuiRender()
	{
	}

	void TestTextRendering::RenderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color)
	{
		s.use();
		s.setVec3("textColor", color);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(m_VAO);

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

				m_Transfrom = glm::translate(glm::mat4(1.0f), glm::vec3(xpos, ypos, 0.0f));
				m_Transfrom = glm::scale(m_Transfrom, glm::vec3(ch.size.x * scale, ch.size.y * scale, 0.0f));
				s.setMat4("transform", m_Transfrom);

				// render glyph texture over quad
				glBindTexture(GL_TEXTURE_2D, ch.textureID);

				// update content of VBO memory
				glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

				// render quad
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				// now advance cursors for next glyph (note that advance is number of 1/64 pixels)

				x += (ch.advance / 64) * scale;
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}