#version 330 core

//property
uniform vec3 u_colour;

//property
uniform sampler2D u_albedo;
//property
uniform sampler2D u_albedo2;

in vec3 v_texCoords;

out vec4 f_colour;

void main()
{
	f_colour = texture(u_albedo, v_texCoords.xy);// vec4(u_colour, 1.0);
}