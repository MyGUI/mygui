
public:
	property Convert< MyGUI::Any >::Type UserData
	{
		Convert< MyGUI::Any >::Type get( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			ObjectHolder * obj = static_cast<ThisType*>(mNative)->getUserData<ObjectHolder>(false);
			return obj ? obj->toObject() : nullptr;
		}
		void set(Convert< MyGUI::Any >::Type _value)
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setUserData( Convert< MyGUI::Any >::From(_value) );
		}
	}

