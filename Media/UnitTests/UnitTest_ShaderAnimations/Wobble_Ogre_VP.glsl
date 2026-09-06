OGRE_NATIVE_GLSL_VERSION_DIRECTIVE

precision highp int;
precision highp float;

in vec4 position;
in vec4 uv0;
in vec4 colour;
uniform mat4 worldViewProj;

out vec4 outUV0;
out vec4 outColor;

uniform vec2 DragOffset; // wobble physics offset from CPU
uniform vec2 InertiaPoint; // point inside widget where drag started, 0..1
uniform float InertiaMode; // 1.0 - resize mode, 0.0 - drag mode
uniform float DragStrength;
uniform float ResizeStrength;
uniform vec2 TexCoordMax;

void main()
{
	outUV0 = uv0;
	outColor = colour;

	vec2 tc = uv0.xy;
	vec2 uvp = tc / TexCoordMax;
	float drageffect;
	if (InertiaMode > 0.5)
	{
		float effect1 = dot(uvp, uvp) * ResizeStrength;
		vec2 diff = uvp - vec2(1.0);
		float effect2 = dot(diff, diff) * ResizeStrength;
		drageffect = min(effect1, effect2);
	}
	else
	{
		vec2 diff = InertiaPoint - uvp;
		drageffect = dot(diff, diff) * DragStrength;
	}

	vec4 vpos = position;
	vpos.x += -DragOffset.x * drageffect;
	vpos.y += DragOffset.y * drageffect;
	gl_Position = worldViewProj * vpos;
}
