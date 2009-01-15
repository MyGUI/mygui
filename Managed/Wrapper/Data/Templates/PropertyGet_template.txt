	public:
		property Convert<#{PropertyType}>::Type #{PropertyName}
		{
			Convert<#{PropertyType}>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<#{PropertyType}>::To( static_cast<ThisType*>(mNative)->get#{PropertyName}() );
			}
			void set(Convert<#{PropertyType}>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->set#{PropertyName}( Convert<#{PropertyType}>::From(_value) );
			}
		}
