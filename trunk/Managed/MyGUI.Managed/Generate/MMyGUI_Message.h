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
		void EndMessage( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->endMessage( );
		}



   	public:
		void EndMessage( ConvertToType<MyGUI::Message::ViewInfo>::Type _result )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->endMessage( ConvertToType<MyGUI::Message::ViewInfo>::ConvertFromValue(_result) );
		}



   	public:
		void SetWindowFade( ConvertToType<bool>::Type _fade )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setWindowFade( ConvertToType<bool>::ConvertFromValue(_fade) );
		}



   	public:
		void SetMessageImage( ConvertToType<size_t>::Type _image )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMessageImage( ConvertToType<size_t>::ConvertFromValue(_image) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetDefaultLayer( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getDefaultLayer( ) );
		}



   	public:
		void SetSmoothShow( ConvertToType<bool>::Type _smooth )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSmoothShow( ConvertToType<bool>::ConvertFromValue(_smooth) );
		}



   	public:
		void SetButton( ConvertToType<MyGUI::Message::ViewInfo>::Type _info )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setButton( ConvertToType<MyGUI::Message::ViewInfo>::ConvertFromValue(_info) );
		}



   	public:
		ConvertToType<MyGUI::Message::ViewInfo>::Type AddButtonName( ConvertToType<const Ogre::UTFString &>::Type _name )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<MyGUI::Message::ViewInfo>::ConvertToValue( static_cast<ThisType*>(mNative)->addButtonName( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_name) ) );
		}



   	public:
		void SetMessage( ConvertToType<const Ogre::UTFString &>::Type _message )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setMessage( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_message) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
