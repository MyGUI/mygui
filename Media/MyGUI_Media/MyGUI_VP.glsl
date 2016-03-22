#version 130

in vec4 position;
in vec4 uv0;
in vec4 colour;

out vec4 outUV0;
out vec4 outColor;

// Texturing vertex program for GLSL
void main()
{
	gl_Position = position;
	outUV0 = uv0;
	outColor = colour;
}
