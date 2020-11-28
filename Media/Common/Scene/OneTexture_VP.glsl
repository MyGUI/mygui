#version 120

/* Basic vertex program for GLSL */

uniform mat4 worldViewProj;
uniform mat4 textureMatrix;

attribute vec4 position;
attribute vec2 uv0;

varying vec2 oUV;

void main()
{
	gl_Position = worldViewProj * position;
	oUV = (textureMatrix * vec4(uv0, 1, 1)).xy;
}
