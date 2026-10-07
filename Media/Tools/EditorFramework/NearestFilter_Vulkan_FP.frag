#version 450
layout(location = 0) in vec4 vColor;
layout(location = 1) in vec2 vTexCoord;
layout(binding = 0) uniform sampler2D Texture;
layout(location = 0) out vec4 fragColor;
void main()
{
	ivec2 size = textureSize(Texture, 0);
	ivec2 pixel = clamp(ivec2(floor(vTexCoord * vec2(size))), ivec2(0), size - ivec2(1));
	fragColor = texelFetch(Texture, pixel, 0) * vColor;
}
