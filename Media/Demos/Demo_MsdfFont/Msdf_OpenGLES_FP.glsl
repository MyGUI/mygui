#version 300 es

precision highp int;
precision highp float;

in vec4 Color;
in vec2 TexCoord;
uniform sampler2D Texture;

out vec4 fragColor;

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

void main(void)
{
	float pxRange = 2.0;

	vec2 msdfUnit = pxRange/vec2(textureSize(Texture, 0));
	vec3 textureColor = texture(Texture, TexCoord).rgb;
	float sigDist = median(textureColor.r, textureColor.g, textureColor.b) - 0.5;
	sigDist *= dot(msdfUnit, 0.5/fwidth(TexCoord));
	float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
	fragColor = vec4(Color.rgb, Color.a * opacity);
}
