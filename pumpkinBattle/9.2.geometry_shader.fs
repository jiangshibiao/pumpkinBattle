#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform float hit;

void main()
{
    vec4 color = texture(texture_diffuse1, TexCoords);
    color = color + vec4(hit,0,0,0);
    FragColor = color;
}

