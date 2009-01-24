/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"


namespace MMyGUI
{

	public ref class Message : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( Message, Widget );

		//InsertPoint

   	public:
		delegate void HandleMessageBoxEnd( Convert<MyGUI::Widget*>::Type _sender, Convert<int>::Type _button );
		event HandleMessageBoxEnd^ MessageBoxEnd
		{
			void add(HandleMessageBoxEnd^ _value)
			{
				mDelegateMessageBoxEnd += _value;
				static_cast<ThisType*>(mNative)->eventMessageBoxEnd = new Delegate2< HandleMessageBoxEnd^, MyGUI::Widget*, int >(mDelegateMessageBoxEnd);
			}
			void remove(HandleMessageBoxEnd^ _value)
			{
				mDelegateMessageBoxEnd -= _value;
				if (mDelegateMessageBoxEnd == nullptr)
					static_cast<ThisType*>(mNative)->eventMessageBoxEnd = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMessageBoxEnd = new Delegate2< HandleMessageBoxEnd^, MyGUI::Widget*, int >(mDelegateMessageBoxEnd);
			}
		}
	private:
		HandleMessageBoxEnd^ mDelegateMessageBoxEnd;




   	public:
		void EndMessage( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->endMessage( );
		}



   	public:
		void EndMessage( Convert<MyGUI::Message::ViewInfo>::Type _result )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->endMessage( Convert<MyGUI::Message::ViewInfo>::From(_result) );
		}



   	public:
		void SetWindowFade( Convert<bool>::Type _fade )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setWindowFade( Convert<bool>::From(_fade) );
		}



   	public:
		void SetMessageImage( Convert<size_t>::Type _image )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMessageImage( Convert<size_t>::From(_image) );
		}



   	public:
		Convert<const std::string &>::Type GetDefaultLayer( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getDefaultLayer( ) );
		}



   	public:
		void SetSmoothShow( Convert<bool>::Type _smooth )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSmoothShow( Convert<bool>::From(_smooth) );
		}



   	public:
		void SetButton( Convert<MyGUI::Message::ViewInfo>::Type _info )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setButton( Convert<MyGUI::Message::ViewInfo>::From(_info) );
		}



   	public:
		Convert<MyGUI::Message::ViewInfo>::Type AddButtonName( Convert<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<MyGUI::Message::ViewInfo>::To( static_cast<ThisType*>(mNative)->addButtonName( Convert<const Ogre::UTFString &>::From(_name) ) );
		}



   	public:
		void SetMessage( Convert<const Ogre::UTFString &>::Type _message )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMessage( Convert<const Ogre::UTFString &>::From(_message) );
		}



	};

} // namespace MMyGUI
