struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};
Texture2D texture0 : register(t0);
float4 main(PS_INPUT input) : SV_Target
{
	uint width, height;
	texture0.GetDimensions(width, height);
	int2 size = int2(width, height);
	int2 pixel = clamp(int2(floor(input.uv * float2(size))), int2(0, 0), size - 1);
	return texture0.Load(int3(pixel, 0)) * input.col;
}
