#version 130

precision highp int;
precision highp float;

uniform sampler2D sampleTexture;

in vec4 outUV0;
in vec4 outColor;

out vec4 fragColor;

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

// Texturing fragment program for GLSL
void main()
{
	float pxRange = 2.0;

	vec2 msdfUnit = pxRange/vec2(textureSize(sampleTexture, 0));
	vec3 textureColor = texture(sampleTexture, outUV0.xy).rgb;
	float sigDist = median(textureColor.r, textureColor.g, textureColor.b) - 0.5;
	sigDist *= dot(msdfUnit, 0.5/fwidth(outUV0.xy));
	float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
	fragColor = vec4(outColor.rgb, outColor.a * opacity);
}
