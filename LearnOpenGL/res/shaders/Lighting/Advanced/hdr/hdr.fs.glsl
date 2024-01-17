#version 330 core

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform float exposure;
uniform bool tone;

void main()
{             
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    vec3 mapped;

    // reinhard tone mapping
    if (tone)
        mapped = hdrColor / (hdrColor + vec3(1.0));
    
    // exposure tone mapping
    else
        mapped = vec3(1.0) - exp(-hdrColor * exposure);
    
    // gamma correction 
    mapped = pow(mapped, vec3(1.0 / gamma));
  
    FragColor = vec4(mapped, 1.0);
}