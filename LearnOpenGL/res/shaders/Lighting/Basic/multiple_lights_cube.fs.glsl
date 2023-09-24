#version 330 core

out vec4 FragColor;

uniform vec3 colorIn;

void main()
{
    FragColor = vec4(colorIn, 1.0); 
}