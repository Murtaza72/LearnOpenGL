#version 330 core

layout (location = 0) in vec2 pos;

out vec2 TexCoords;

uniform mat4 transform;
uniform mat4 projection;

void main()
{
    gl_Position = projection * transform * vec4(pos, 0.0, 1.0);
    TexCoords.x = pos.x;
    TexCoords.y = 1.0 - pos.y;
}