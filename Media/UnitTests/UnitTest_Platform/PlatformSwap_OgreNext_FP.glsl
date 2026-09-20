#version 150
in vec2 outUV;
in vec4 outCol;
uniform sampler2D sampler0;
out vec4 fragColour;
void main()
{
	fragColour = (texture(sampler0, outUV) * outCol).bgra;
}
