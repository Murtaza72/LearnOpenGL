#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform bool bloom;
uniform float exposure;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(scene, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
 
    if(bloom)
        hdrColor += bloomColor; // additive blending
    
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    
    // gamma correction
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}