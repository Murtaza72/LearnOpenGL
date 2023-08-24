#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float change;

void main()
{
	FragColor = mix(texture(tex1, TexCoord), texture(tex2, vec2( -TexCoord.x, TexCoord.y)), change) * vec4(ourColor, 1.0f);
};