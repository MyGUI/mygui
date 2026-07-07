#version 150

in vec2 outUV;
in vec4 outCol;

uniform sampler2D sampler0;

out vec4 fragColour;

float median(float r, float g, float b)
{
    return max(min(r, g), min(max(r, g), b));
}

void main()
{
    float pxRange = 2.0;

    vec2 msdfUnit = pxRange / vec2(textureSize(sampler0, 0));
    vec4 texColour = texture(sampler0, outUV);

    float sigDist = median(texColour.r, texColour.g, texColour.b) - 0.5;
    sigDist *= dot(msdfUnit, 0.5 / fwidth(outUV));

    float opacity = clamp(sigDist + 0.5, 0.0, 1.0);
    fragColour = vec4(outCol.rgb, outCol.a * opacity * texColour.a);
}
