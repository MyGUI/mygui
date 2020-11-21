
/* Basic fragment program using one texture. */

void OneTexture_FP(float4 position          : POSITION, 
				   float2 uv                : TEXCOORD0,
				   out float4 colour        : COLOR,
				   uniform sampler2D texMap : register(s0))
{
	colour = tex2D(texMap, uv);
}
