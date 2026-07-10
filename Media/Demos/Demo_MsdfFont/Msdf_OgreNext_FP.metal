#include <metal_stdlib>
using namespace metal;

struct VertexOut
{
    float4 position [[position]];
    float2 uv;
    float4 col;
};

static float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

fragment float4 fragment_main(VertexOut in [[stage_in]],
                              texture2d<float> tex [[texture(0)]])
{
    constexpr sampler linearSampler(coord::normalized, min_filter::linear,
                                    mag_filter::linear, mip_filter::linear);

    float pxRange = 2.0;
    float2 texSize = float2(tex.get_width(), tex.get_height());
    float2 msdfUnit = pxRange / texSize;

    float4 texColour = tex.sample(linearSampler, in.uv);
    float sigDist = median(texColour.r, texColour.g, texColour.b) - 0.5;
    sigDist *= dot(msdfUnit, 0.5 / max(fwidth(in.uv), 1e-6));

    float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
    return float4(in.col.rgb, in.col.a * opacity * texColour.a);
}
