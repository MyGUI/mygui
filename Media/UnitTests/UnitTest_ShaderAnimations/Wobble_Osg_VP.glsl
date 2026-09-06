#version 150

precision highp int;
precision highp float;

in vec4 osg_Vertex;
in vec4 osg_Color;
in vec2 osg_MultiTexCoord0;

uniform mat4 osg_ModelViewProjectionMatrix;
uniform float YScale;
uniform vec2 DragOffset; // wobble physics offset from CPU
uniform vec2 InertiaPoint; // point inside widget where drag started, 0..1
uniform float InertiaMode; // 1.0 - resize mode, 0.0 - drag mode
uniform float DragStrength;
uniform float ResizeStrength;
uniform vec2 TexCoordMax;

out vec4 Color;
out vec2 TexCoord;

void main()
{
	TexCoord = osg_MultiTexCoord0;
	Color = osg_Color;

	// same displacement as WobbleNodeAnimator::buildQuadVertex, but computed per vertex on GPU
	vec2 uv = osg_MultiTexCoord0 / TexCoordMax;
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

	vec4 vpos = osg_Vertex;
	vpos.x += -DragOffset.x * drageffect;
	vpos.y += DragOffset.y * drageffect;
	vpos.y *= YScale;
	gl_Position = osg_ModelViewProjectionMatrix * vpos;
}
