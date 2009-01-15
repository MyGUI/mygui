	public:
		void #{NewFunctionName}( Convert<#{ValueType1}>::Type #{ValueName1}, Convert<#{ValueType2}>::Type #{ValueName2} )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->#{FunctionName}( Convert<#{ValueType1}>::From(#{ValueName1}), Convert<#{ValueType2}>::From(#{ValueName2}) );
		}
