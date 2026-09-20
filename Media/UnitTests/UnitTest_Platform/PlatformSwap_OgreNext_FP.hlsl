struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};
sampler sampler0 : register(s0);
Texture2D texture0 : register(t0);
float4 main(PS_INPUT input) : SV_Target
{
	return (texture0.Sample(sampler0, input.uv) * input.col).bgra;
}
