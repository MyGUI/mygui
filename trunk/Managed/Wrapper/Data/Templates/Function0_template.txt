	public:
		void #{NewFunctionName}( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->#{FunctionName}( );
		}
