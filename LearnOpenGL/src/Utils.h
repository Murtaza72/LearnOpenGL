#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include <GL/glew.h>

#include "stb_image/stb_image.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(std::string_view function, const char* file, int line);
unsigned int LoadTexture(const std::string& path);
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
unsigned int LoadCubeMap(std::vector<std::string> faces, std::string relPath);