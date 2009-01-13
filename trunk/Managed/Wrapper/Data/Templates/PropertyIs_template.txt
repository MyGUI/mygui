	public:
		property #{PropertyType} #{PropertyName}
		{
			#{PropertyType} get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return static_cast<ThisType*>(mNative)->is#{PropertyName}( );
			}
			void set(#{PropertyType} _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->set#{PropertyName}(_value);
			}
		}
