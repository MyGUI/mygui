@piece( Common_Matrix_DeclUnpackMatrix4x4 )
float4x4 UNPACK_MAT4( Buffer<float4> matrixBuf, uint pixelIdx )
{
	float4 row1 = matrixBuf.Load( int((pixelIdx) << 2u) );
	float4 row2 = matrixBuf.Load( int(((pixelIdx) << 2u) + 1u) );
	float4 row3 = matrixBuf.Load( int(((pixelIdx) << 2u) + 2u) );
	float4 row4 = matrixBuf.Load( int(((pixelIdx) << 2u) + 3u) );

	return float4x4( row1, row2, row3, row4 );
}
@end

@piece( Common_Matrix_DeclUnpackMatrix3x4 )
float3x4 UNPACK_MAT3x4( Buffer<float4> matrixBuf, uint pixelIdx )
{
	float4 row1 = matrixBuf.Load( int((pixelIdx) << 2u) );
	float4 row2 = matrixBuf.Load( int(((pixelIdx) << 2u) + 1u) );
	float4 row3 = matrixBuf.Load( int(((pixelIdx) << 2u) + 2u) );

	return float3x4( row1, row2, row3 );
}
@end
