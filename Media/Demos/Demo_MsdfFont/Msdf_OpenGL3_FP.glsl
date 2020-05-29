#version 130
in vec4 Color;
in vec2 TexCoord;
uniform sampler2D Texture;

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

void main(void)
{
	float pxRange = 2.0;

	vec2 msdfUnit = pxRange/vec2(textureSize(Texture, 0));
	vec3 sample = texture2D(Texture, TexCoord).rgb;
	float sigDist = median(sample.r, sample.g, sample.b) - 0.5;
	sigDist *= dot(msdfUnit, 0.5/fwidth(TexCoord));
	float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
	gl_FragColor = vec4(Color.rgb, Color.a * opacity);
}
