OGRE_NATIVE_GLSL_VERSION_DIRECTIVE
precision highp int;
precision highp float;
in vec4 outUV0;
in vec4 outColor;
uniform sampler2D sampleTexture;
out vec4 fragColor;
void main()
{
	ivec2 size = textureSize(sampleTexture, 0);
	ivec2 pixel = clamp(ivec2(floor(outUV0.xy * vec2(size))), ivec2(0), size - ivec2(1));
	fragColor = texelFetch(sampleTexture, pixel, 0) * outColor;
}
