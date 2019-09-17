#version 330 core

in vec3 v_Position;

out vec4 f_Colour;

void main()
{
	vec3 colour = v_Position / 4.0f;
	f_Colour = vec4(v_Position.rgb, 1.0);
}