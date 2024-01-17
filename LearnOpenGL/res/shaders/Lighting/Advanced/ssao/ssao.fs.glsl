#version 330 core

out float FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];
uniform mat4 projection;

const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0); // screen = 800x600

void main()
{
	vec3 fragPos   = texture(gPosition, TexCoords).xyz;
	vec3 normal    = texture(gNormal, TexCoords).rgb;
	vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz;
}