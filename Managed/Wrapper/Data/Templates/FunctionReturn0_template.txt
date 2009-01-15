	public:
		Convert<#{ValueTypeReturn}>::Type #{NewFunctionName}( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<#{ValueTypeReturn}>::To( static_cast<ThisType*>(mNative)->#{FunctionName}( ) );
		}
