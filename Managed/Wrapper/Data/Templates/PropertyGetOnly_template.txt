	public:
		property Convert<#{PropertyType}>::Type #{PropertyName}
		{
			Convert<#{PropertyType}>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<#{PropertyType}>::To( static_cast<ThisType*>(mNative)->get#{PropertyName}() );
			}
		}
