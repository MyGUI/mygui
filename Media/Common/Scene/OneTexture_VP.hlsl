
/* Basic vertex program */

void OneTexture_VP(float4 position : POSITION,
				   float2 uv		  : TEXCOORD0,
				   out float4 oPosition : POSITION,
				   out float2 oUV	   : TEXCOORD0,
				   uniform float4x4 worldViewProj,
				   uniform float4x4 textureMatrix)
{
	oPosition = mul(worldViewProj, position);
	oUV = mul(textureMatrix, float4(uv,1,1)).xy;
}
