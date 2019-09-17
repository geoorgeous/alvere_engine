#version 330 core

//uniform struct {
//	mat4 u_
//} rendererInput;

uniform mat4 u_projectionMatrix;
uniform mat4 u_viewMatrix;
uniform mat4 u_modelMatrix;

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_texCoords;

out vec3 v_texCoords;

void main()
{
	v_texCoords = a_texCoords;

	gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * vec4(a_position, 1.0);
}