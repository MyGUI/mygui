#include <metal_stdlib>
using namespace metal;

struct VertexOut
{
	float4 position [[position]];
	float2 uv;
	float4 col;
};

fragment float4 fragment_main(VertexOut in [[stage_in]], texture2d<float> tex [[texture(0)]])
{
	int2 size = int2(tex.get_width(), tex.get_height());
	int2 pixel = clamp(int2(floor(in.uv * float2(size))), int2(0), size - int2(1));
	return tex.read(uint2(pixel), 0) * in.col;
}
