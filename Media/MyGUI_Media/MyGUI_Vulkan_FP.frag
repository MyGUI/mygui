#version 450

layout(location = 0) in vec4 vColor;
layout(location = 1) in vec2 vTexCoord;

layout(binding = 0) uniform sampler2D Texture;

layout(location = 0) out vec4 fragColor;

void main()
{
	fragColor = texture(Texture, vTexCoord) * vColor;
}
