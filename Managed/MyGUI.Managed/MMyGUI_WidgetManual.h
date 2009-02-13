
public:
	property System::Object^ UserData
	{
		System::Object^ get( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return mUserData;
		}
		void set(System::Object^ _value)
		{
			MMYGUI_CHECK_NATIVE(mNative);
			mUserData = _value;
		}
	}

