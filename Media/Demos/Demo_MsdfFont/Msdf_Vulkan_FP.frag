#version 450

layout(location = 0) in vec4 vColor;
layout(location = 1) in vec2 vTexCoord;

layout(binding = 0) uniform sampler2D Texture;

layout(location = 0) out vec4 fragColor;

float median(float r, float g, float b)
{
	return max(min(r, g), min(max(r, g), b));
}

void main()
{
	float pxRange = 2.0;

	vec2 msdfUnit = pxRange / vec2(textureSize(Texture, 0));
	vec4 textureColor = texture(Texture, vTexCoord);
	float sigDist = median(textureColor.r, textureColor.g, textureColor.b) - 0.5;
	sigDist *= dot(msdfUnit, 0.5 / max(fwidth(vTexCoord), 1e-6));
	float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
	fragColor = vec4(vColor.rgb, vColor.a * opacity * textureColor.a);
}
