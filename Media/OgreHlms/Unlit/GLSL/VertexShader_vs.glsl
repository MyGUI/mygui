@insertpiece( SetCrossPlatformSettings )

out gl_PerVertex
{
	vec4 gl_Position;
};

layout(std140) uniform;

@insertpiece( Common_Matrix_DeclUnpackMatrix4x4 )

in vec4 vertex;
@property( hlms_colour )in vec4 colour;@end

@foreach( hlms_uv_count, n )
in vec@value( hlms_uv_count@n ) uv@n;@end

in uint drawId;

@insertpiece( custom_vs_attributes )

out block
{
@insertpiece( VStoPS_block )
} outVs;

// START UNIFORM DECLARATION
@insertpiece( PassDecl )
@insertpiece( InstanceDecl )
layout(binding = 0) uniform samplerBuffer worldMatBuf;
@property( texture_matrix )layout(binding = 1) uniform samplerBuffer animationMatrixBuf;@end
@insertpiece( custom_vs_uniformDeclaration )
// END UNIFORM DECLARATION

void main()
{
	@insertpiece( custom_vs_preExecution )
	//uint drawId = 1;
	mat4 worldViewProj;
	worldViewProj = UNPACK_MAT4( worldMatBuf, drawId );

@property( !hlms_dual_paraboloid_mapping )
	gl_Position = worldViewProj * vertex;
@end

@property( hlms_dual_paraboloid_mapping )
	//Dual Paraboloid Mapping
	gl_Position.w	= 1.0f;
	gl_Position.xyz	= (worldViewProj * vertex).xyz;
	float L = length( gl_Position.xyz );
	gl_Position.z	+= 1.0f;
	gl_Position.xy	/= gl_Position.z;
	gl_Position.z	= (L - NearPlane) / (FarPlane - NearPlane);
@end

@property( !hlms_shadowcaster )
@property( hlms_colour )	outVs.colour = colour;@end

@property( texture_matrix )	mat4 textureMatrix;@end

@foreach( out_uv_count, n )
	@property( out_uv@_texture_matrix )textureMatrix = UNPACK_MAT4( animationMatrixBuf, (instance.materialIdx[drawId].x << 4u) + @value( out_uv@n_tex_unit ) );@end
	outVs.uv@value( out_uv@n_out_uv ).@insertpiece( out_uv@n_swizzle ) = uv@value( out_uv@n_source_uv ).xy @property( out_uv@_texture_matrix ) * textureMatrix@end ;@end

	outVs.drawId = drawId;

@end @property( hlms_shadowcaster )
	float shadowConstantBias = uintBitsToFloat( instance.materialIdx[drawId].y );
	//Linear depth
	outVs.depth	= (gl_Position.z - pass.depthRange.x + shadowConstantBias * pass.depthRange.y) * pass.depthRange.y;

	//We can't make the depth buffer linear without Z out in the fragment shader;
	//however we can use a cheap approximation ("pseudo linear depth")
	//see http://www.yosoygames.com.ar/wp/2014/01/linear-depth-buffer-my-ass/
	gl_Position.z = gl_Position.z * (gl_Position.w * pass.depthRange.y);
@end
	@insertpiece( custom_vs_posExecution )
}
