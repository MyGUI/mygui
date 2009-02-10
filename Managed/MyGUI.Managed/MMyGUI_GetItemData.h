   	public:
		Convert< MyGUI::Any >::Type GetItemData( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			ObjectHolder * obj = static_cast<ThisType*>(mNative)->getItemData < ObjectHolder > ( false );
			return obj ? obj->toObject() : nullptr;
		}
