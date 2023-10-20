#pragma once

#include "Shader.h"

struct Character
{
	unsigned int textureID;  // ID handle of the glyph texture
	glm::ivec2   size;       // Size of glyph
	glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
	unsigned int advance;    // Offset to advance to next glyph
};

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();
	void Load(std::string font, unsigned int fontSize);
	void RenderText(std::string text, float x, float y, float scale, glm::vec3 color);

private:
	unsigned int m_VAO, m_VBO;
	std::map<char, Character> m_Characters;
	Shader m_Shader;
	glm::mat4 m_Transfrom;
};