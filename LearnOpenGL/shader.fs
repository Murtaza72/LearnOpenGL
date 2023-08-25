#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float change;

void main()
{
	FragColor = mix(texture(tex1, TexCoord), texture(tex2, TexCoord), change);
};