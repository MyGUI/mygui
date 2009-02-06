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
		delegate void HandleMessageBoxResult( Convert<MyGUI::Message *>::Type _sender, Convert<MyGUI::MessageStyle>::Type _result );
		event HandleMessageBoxResult^ EventMessageBoxResult
		{
			void add(HandleMessageBoxResult^ _value)
			{
				mDelegateMessageBoxResult += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventMessageBoxResult =
					static_cast< MyGUI::delegates::IDelegate2< MyGUI::Message *, MyGUI::MessageStyle > *>(
						new Delegate2< HandleMessageBoxResult^, MyGUI::Message *, MyGUI::MessageStyle >(mDelegateMessageBoxResult) );
			}
			void remove(HandleMessageBoxResult^ _value)
			{
				mDelegateMessageBoxResult -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateMessageBoxResult == nullptr)
					static_cast<ThisType*>(mNative)->eventMessageBoxResult = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventMessageBoxResult =
						static_cast< MyGUI::delegates::IDelegate2< MyGUI::Message *, MyGUI::MessageStyle > *>(
							new Delegate2< HandleMessageBoxResult^, MyGUI::Message *, MyGUI::MessageStyle >(mDelegateMessageBoxResult) );
			}
		}
	private:
		HandleMessageBoxResult^ mDelegateMessageBoxResult;




   	public:
		void SetMessageButton( Convert< MyGUI::MessageStyle >::Type _button )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMessageButton( Convert< MyGUI::MessageStyle >::From(_button) );
		}



   	public:
		void EndMessage( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->endMessage( );
		}



   	public:
		void EndMessage( Convert< MyGUI::MessageStyle >::Type _result )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->endMessage( Convert< MyGUI::MessageStyle >::From(_result) );
		}



   	public:
		void SetWindowFade( Convert<bool>::Type _fade )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setWindowFade( Convert<bool>::From(_fade) );
		}



   	public:
		void SetMessageImage( Convert< MyGUI::MessageStyle >::Type _image )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMessageImage( Convert< MyGUI::MessageStyle >::From(_image) );
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
		Convert< MyGUI::MessageStyle >::Type AddButtonName( Convert<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MessageStyle >::To( static_cast<ThisType*>(mNative)->addButtonName( Convert<const Ogre::UTFString &>::From(_name) ) );
		}



   	public:
		void SetMessage( Convert<const Ogre::UTFString &>::Type _message )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMessage( Convert<const Ogre::UTFString &>::From(_message) );
		}



	};

} // namespace MMyGUI
