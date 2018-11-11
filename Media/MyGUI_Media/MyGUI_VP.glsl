#version 130

precision highp int;
precision highp float;

in vec4 position;
in vec4 uv0;
in vec4 colour;

out vec4 outUV0;
out vec4 outColor;

out gl_PerVertex { vec4 gl_Position; };

// Texturing vertex program for GLSL ES
void main()
{
	gl_Position = position;
	outUV0 = uv0;
	outColor = colour;
}

