OGRE_NATIVE_GLSL_VERSION_DIRECTIVE

precision highp int;
precision highp float;

in vec4 position;
in vec4 uv0;
in vec4 colour;
uniform mat4 worldViewProj;

out vec4 outUV0;
out vec4 outColor;

void main()
{
	gl_Position = worldViewProj * position;
	outUV0 = uv0;
	outColor = colour;
}
