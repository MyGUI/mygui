   	public:
		Convert< MyGUI::Any >::Type GetItemDataAt( Convert<size_t>::Type _index )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			ObjectHolder * obj = static_cast<ThisType*>(mNative)->getItemDataAt < ObjectHolder > ( Convert<size_t>::From(_index) );
			return obj ? obj->toObject() : nullptr;
		}
