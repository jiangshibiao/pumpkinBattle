#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, normalize(TexCoords));
	// FragColor = vec4(TexCoords ,1);
}