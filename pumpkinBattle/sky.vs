#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
	vec4 intermediate = view * vec4(aPos, 1.0);
	intermediate = projection * vec4(intermediate.xyz, 1.0f);
    gl_Position = intermediate.xyww;
}