void main(
	uniform Texture2D<float4> sampleTexture : register(t0),
	in float4 inPosition : SV_POSITION,
	in float4 inColor : TEXCOORD0,
	in float2 inTexcoord : TEXCOORD1,
	out float4 Out : SV_TARGET)
{
	uint width, height;
	sampleTexture.GetDimensions(width, height);
	int2 size = int2(width, height);
	int2 pixel = clamp(int2(floor(inTexcoord * float2(size))), int2(0, 0), size - 1);
	Out = sampleTexture.Load(int3(pixel, 0)) * inColor;
}
