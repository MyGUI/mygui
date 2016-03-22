#version 130

uniform sampler2D sampler;

in vec4 outUV0;
in vec4 outColor;

out vec4 fragColor;

// Texturing fragment program for GLSL
void main()
{
	fragColor = outColor * texture(sampler, outUV0.xy);
}
