#version 130

precision highp int;
precision highp float;

varying vec4 Color;
varying vec2 TexCoord;

uniform sampler2D Texture;

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

// Multi-channel signed distance field font rendering
void main(void)
{
	float pxRange = 2.0;

	vec2 msdfUnit = pxRange/vec2(textureSize(Texture, 0));
	vec4 textureColor = texture2D(Texture, TexCoord);
	float sigDist = median(textureColor.r, textureColor.g, textureColor.b) - 0.5;
	sigDist *= dot(msdfUnit, 0.5/max(fwidth(TexCoord), 1e-6));
	float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
	gl_FragColor = vec4(Color.rgb, Color.a * opacity * textureColor.a);
}
