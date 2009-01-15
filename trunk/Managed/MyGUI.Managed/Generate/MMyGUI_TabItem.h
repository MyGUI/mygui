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

	public ref class TabItem : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( TabItem, Widget );

		//InsertPoint

   	public:
		void RemoveItem( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItem( );
		}



   	public:
		void SetItemSelected( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemSelected( );
		}



   	public:
		property ConvertToType<const Ogre::UTFString &>::Type ItemName
		{
			ConvertToType<const Ogre::UTFString &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemName() );
			}
			void set(ConvertToType<const Ogre::UTFString &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemName( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<int>::Type ButtonWidth
		{
			ConvertToType<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getButtonWidth() );
			}
			void set(ConvertToType<int>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setButtonWidth( ConvertToType<int>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<const Ogre::UTFString &>::Type Caption
		{
			ConvertToType<const Ogre::UTFString &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<const Ogre::UTFString &>::ConvertToValue( static_cast<ThisType*>(mNative)->getCaption() );
			}
			void set(ConvertToType<const Ogre::UTFString &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setCaption( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
