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

	public ref class MenuItem : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( MenuItem, Widget );

		//InsertPoint

   	public:
		ConvertToType< MyGUI::MenuCtrl * >::Type GetItemChild( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuCtrl * >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemChild( ) );
		}



   	public:
		ConvertToType< MyGUI::MenuCtrl * >::Type GetMenuCtrlParent( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuCtrl * >::ConvertToValue( static_cast<ThisType*>(mNative)->getMenuCtrlParent( ) );
		}



   	public:
		void HideItemChild( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->hideItemChild( );
		}



   	public:
		void ShowItemChild( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->showItemChild( );
		}



   	public:
		property ConvertToType< MyGUI::MenuItemType >::Type ItemType
		{
			ConvertToType< MyGUI::MenuItemType >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType< MyGUI::MenuItemType >::ConvertToValue( static_cast<ThisType*>(mNative)->getItemType() );
			}
			void set(ConvertToType< MyGUI::MenuItemType >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemType( ConvertToType< MyGUI::MenuItemType >::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType< MyGUI::MenuCtrl * >::Type CreateItemChild( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType< MyGUI::MenuCtrl * >::ConvertToValue( static_cast<ThisType*>(mNative)->createItemChild( ) );
		}



   	public:
		ConvertToType<size_t>::Type GetItemIndex( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<size_t>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemIndex( ) );
		}



   	public:
		property ConvertToType<const std::string &>::Type ItemId
		{
			ConvertToType<const std::string &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getItemId() );
			}
			void set(ConvertToType<const std::string &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemId( ConvertToType<const std::string &>::ConvertFromValue(_value) );
			}
		}



   	public:
		void RemoveItem( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItem( );
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
		void SetCaption( ConvertToType<const Ogre::UTFString &>::Type _caption )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCaption( ConvertToType<const Ogre::UTFString &>::ConvertFromValue(_caption) );
		}



   	public:
		ConvertToType<const std::string &>::Type GetTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



	};

} // namespace MMyGUI
