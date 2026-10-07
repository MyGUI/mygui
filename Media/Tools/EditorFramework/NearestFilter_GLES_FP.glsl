#version 300 es
precision highp int;
precision highp float;
in vec4 Color;
in vec2 TexCoord;
uniform sampler2D Texture;
out vec4 fragColor;
void main()
{
	ivec2 size = textureSize(Texture, 0);
	ivec2 pixel = clamp(ivec2(floor(TexCoord * vec2(size))), ivec2(0), size - ivec2(1));
	fragColor = texelFetch(Texture, pixel, 0) * Color;
}
