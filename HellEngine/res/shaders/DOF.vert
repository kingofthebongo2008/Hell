﻿#version 330

in vec3 in_position;
in vec2 in_texcoord;

out vec2 TexCoords;

void main(void)
{
	TexCoords = in_texcoord;
	gl_Position = vec4(in_position, 1.0);
}	