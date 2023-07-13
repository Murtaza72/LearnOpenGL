#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main()
{
	FragColor = mix(texture(tex1, TexCoord), texture(tex2, TexCoord), 0.5) * vec4(ourColor, 1.0);
};