#version 330 core

uniform mat4 u_ProjectionView;

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoords;
layout(location = 2) in vec4 a_Colour;
layout(location = 3) in int a_TextureIndex;

out vec3 v_Position;
out vec2 v_TexCoords;
out vec4 v_Colour;
flat out int v_TextureIndex;

void main()
{
	v_Position = a_Position;
	v_TexCoords = a_TexCoords;
	v_Colour = a_Colour;
	v_TextureIndex = a_TextureIndex;

	gl_Position = u_ProjectionView * vec4(a_Position, 1.0);
}