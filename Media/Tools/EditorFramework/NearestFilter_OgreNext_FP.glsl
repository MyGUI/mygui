#version 150
in vec2 outUV;
in vec4 outCol;
uniform sampler2D sampler0;
out vec4 fragColour;
void main()
{
	ivec2 size = textureSize(sampler0, 0);
	ivec2 pixel = clamp(ivec2(floor(outUV * vec2(size))), ivec2(0), size - ivec2(1));
	fragColour = texelFetch(sampler0, pixel, 0) * outCol;
}
