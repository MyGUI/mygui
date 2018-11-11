#version 130

precision highp int;
precision highp float;

uniform sampler2D sampler;

in vec4 outUV0;
in vec4 outColor;

// Texturing fragment program for GLSL ES
void main()
{
	gl_FragColor = outColor * texture(sampler, outUV0.xy);
}
