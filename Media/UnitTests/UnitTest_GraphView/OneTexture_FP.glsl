#version 120

/* Basic fragment program using one texture. */

uniform sampler2D texMap;

varying vec2 oUV;

void main()
{
	gl_FragColor = texture2D(texMap, oUV);
}
