#version 120

/* Basic vertex program for GLSL */

uniform mat4 worldViewProj;

attribute vec4 position;
attribute vec2 uv0;

varying vec4 gl_Position;
varying vec2 oUV;

void main()
{
	gl_Position = worldViewProj * position;
	oUV = uv0;
}
