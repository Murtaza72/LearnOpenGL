#pragma once

#include <string_view>

#include "Shader.h"

class Shader;

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(std::string_view function, const char* file, int line);

unsigned int LoadTexture(const std::string& path);
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);
unsigned int LoadCubeMap(std::vector<std::string> faces, std::string relPath);

void RenderScene(const Shader& shader);
void RenderPlane();
void RenderCube();
void RenderQuad();

void DisplayFramebufferTexture(unsigned int textureID, int type = GL_TEXTURE_2D);

unsigned int GenerateTangents();