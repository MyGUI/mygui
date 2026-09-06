#version 300 es

precision highp int;
precision highp float;

in vec4 Color;
in vec2 TexCoord;
uniform sampler2D Texture;
uniform float Progress; // 0..1, burn front position
uniform vec2 TexCoordMax; // TexCoord extent of actual content inside POT texture
uniform float NoiseScale;
out vec4 fragColor;

float hash(vec2 p)
{
	return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453);
}

float vnoise(vec2 p)
{
	vec2 i = floor(p);
	vec2 f = fract(p);
	f = f * f * (3.0 - 2.0 * f);
	return mix(mix(hash(i), hash(i + vec2(1.0, 0.0)), f.x), mix(hash(i + vec2(0.0, 1.0)), hash(i + vec2(1.0, 1.0)), f.x), f.y);
}

float fbm(vec2 p)
{
	return 0.5 * vnoise(p) + 0.25 * vnoise(p * 2.13) + 0.125 * vnoise(p * 4.71) + 0.0625 * vnoise(p * 9.37);
}

void main(void)
{
	vec2 uv = TexCoord / TexCoordMax;
	vec4 texel = texture(Texture, TexCoord);

	// positive distance to burn front, higher noise burns later
	float e = fbm(uv * NoiseScale) - Progress;

	if (e < 0.0)
	{
		// burned away
		discard;
	}
	else if (e < 0.04)
	{
		// glowing rim
		float t = e / 0.04;
		fragColor = vec4(mix(vec3(1.0, 0.95, 0.55), vec3(1.0, 0.35, 0.0), t), texel.a);
	}
	else if (e < 0.15)
	{
		// charred edge
		fragColor = texel * (e / 0.15);
	}
	else
	{
		// intact
		fragColor = texel * Color;
	}
}
