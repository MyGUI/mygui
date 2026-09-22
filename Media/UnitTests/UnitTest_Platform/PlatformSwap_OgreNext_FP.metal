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
	constexpr sampler linearSampler(coord::normalized, address::clamp_to_edge,
		min_filter::linear, mag_filter::linear, mip_filter::none);
	return (in.col * tex.sample(linearSampler, in.uv)).bgra;
}
