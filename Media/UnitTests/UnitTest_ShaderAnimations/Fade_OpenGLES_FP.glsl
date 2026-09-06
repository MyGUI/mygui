#version 300 es

precision highp int;
precision highp float;

in vec4 Color;
in vec2 TexCoord;
uniform sampler2D Texture;
uniform float Alpha; // 0..1, fade progress
uniform float FadeType; // 0 - random squares, 1 - random not-squares, 2 - from center, 3 - TV
uniform vec2 TexCoordMax;
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

void main(void)
{
	vec2 uv = TexCoord / TexCoordMax;

	float speed = 1.0;
	if (FadeType < 0.5)
	{
		// random squares
		speed = floor(hash(floor(uv * 64.0)) * 4.0) + 1.0;
	}
	else if (FadeType < 1.5)
	{
		// random not-squares
		speed = vnoise(uv * 8.0) * 3.0 + 1.0;
	}
	else if (FadeType < 2.5)
	{
		// from center
		vec2 p = (uv - 0.5) * 2.0;
		speed = length(p);
	}
	else
	{
		// TV
		vec2 p = (uv - 0.5) * 2.0;
		speed = sqrt(p.x * p.x * p.x * p.x + p.y * p.y);
	}

	float alpha = pow(max(Alpha, 0.0001), max(speed, 0.0001));
	fragColor = texture(Texture, TexCoord) * (Color * vec4(1.0, 1.0, 1.0, alpha));
}
