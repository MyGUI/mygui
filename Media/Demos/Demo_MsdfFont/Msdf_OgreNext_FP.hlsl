struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 uv  : TEXCOORD0;
};

sampler sampler0 : register(s0);
Texture2D texture0 : register(t0);

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

float4 main(PS_INPUT input) : SV_Target
{
    float pxRange = 2.0;

    float w;
    float h;
    texture0.GetDimensions(w, h);
    float2 msdfUnit = pxRange / float2(w, h);

    float4 texColour = texture0.Sample(sampler0, input.uv);
    float sigDist = median(texColour.r, texColour.g, texColour.b) - 0.5;
    sigDist *= dot(msdfUnit, 0.5 / fwidth(input.uv));

    float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
    return float4(input.col.rgb, input.col.a * opacity * texColour.a);
}
