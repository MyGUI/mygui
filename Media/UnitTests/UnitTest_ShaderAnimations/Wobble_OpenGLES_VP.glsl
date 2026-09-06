#version 300 es
out vec4 Color;
out vec2 TexCoord;
in vec3 VertexPosition;
in vec4 VertexColor;
in vec2 VertexTexCoord;
uniform float YScale;
uniform vec2 DragOffset; // wobble physics offset from CPU
uniform vec2 InertiaPoint; // point inside widget where drag started, 0..1
uniform float InertiaMode; // 1.0 - resize mode, 0.0 - drag mode
uniform float DragStrength;
uniform float ResizeStrength;
uniform vec2 TexCoordMax;
void main()
{
	TexCoord = VertexTexCoord;
	Color = VertexColor;

	// same displacement as WobbleNodeAnimator::buildQuadVertex, but computed per vertex on GPU
	vec2 uv = VertexTexCoord / TexCoordMax;
	float drageffect;
	if (InertiaMode > 0.5)
	{
		float effect1 = dot(uv, uv) * ResizeStrength;
		vec2 diff = uv - vec2(1.0);
		float effect2 = dot(diff, diff) * ResizeStrength;
		drageffect = min(effect1, effect2);
	}
	else
	{
		vec2 diff = InertiaPoint - uv;
		drageffect = dot(diff, diff) * DragStrength;
	}

	vec4 vpos = vec4(VertexPosition, 1.0);
	vpos.x += -DragOffset.x * drageffect;
	vpos.y += DragOffset.y * drageffect;
	vpos.y *= YScale;
	gl_Position = vpos;
}
