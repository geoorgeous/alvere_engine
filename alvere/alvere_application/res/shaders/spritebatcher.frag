#version 330 core

uniform sampler2D u_Textures[16];

in vec3 v_Position;
in vec2 v_TexCoords;
in vec4 v_Colour;
flat in int v_TextureIndex;

out vec4 f_Colour;

void main()
{
	f_Colour = texture(u_Textures[v_TextureIndex], v_TexCoords) * v_Colour;
	//f_Colour = vec4(v_Position, 1.0f);
	//f_Colour = vec4(v_TexCoords.x, 0.0f, v_TexCoords.y, 1.0f);
	//f_Colour = vec4(intBitsToFloat(v_TextureIndex) * 0.5f, 0.0f, 0.0f, 1.0f);
	//f_Colour = v_Colour;
}