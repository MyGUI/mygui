float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

void main(
	uniform Texture2D<float4> sampleTexture : register(t0),
	uniform SamplerState sampleSampler : register(s0),
	in float4 inPosition : SV_POSITION,
	in float4 inColor : TEXCOORD0,
	in float2 inTexcoord : TEXCOORD1,
	out float4 Out : SV_TARGET )
{
	float pxRange = 2.0;
	float w;
	float h;
	sampleTexture.GetDimensions(w, h);
	float2 msdfUnit = pxRange/float2(w, h);
	float3 textureColor = sampleTexture.SampleLevel(sampleSampler, inTexcoord, 0).rgb;
	float sigDist = median(textureColor.r, textureColor.g, textureColor.b) - 0.5;
	sigDist *= dot(msdfUnit, 0.5/fwidth(inTexcoord));
	float opacity = clamp(sigDist + 0.5, 0.0, 1.0);

	Out = float4(inColor.rgb, inColor.a * opacity);
}
