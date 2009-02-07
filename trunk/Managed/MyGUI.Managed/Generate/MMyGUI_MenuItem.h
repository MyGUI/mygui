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
		MMYGUI_DECLARE_DERIVED( MenuItem, MenuItem, Widget );

		

		//InsertPoint

   	public:
		Convert< MyGUI::MenuCtrl * >::Type GetItemChild( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuCtrl * >::To( static_cast<ThisType*>(mNative)->getItemChild( ) );
		}



   	public:
		Convert< MyGUI::MenuCtrl * >::Type GetMenuCtrlParent( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuCtrl * >::To( static_cast<ThisType*>(mNative)->getMenuCtrlParent( ) );
		}



   	public:
		void SetItemChildVisible( Convert<bool>::Type _visible )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setItemChildVisible( Convert<bool>::From(_visible) );
		}



   	public:
		property Convert< MyGUI::MenuItemType >::Type ItemType
		{
			Convert< MyGUI::MenuItemType >::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert< MyGUI::MenuItemType >::To( static_cast<ThisType*>(mNative)->getItemType() );
			}
			void set(Convert< MyGUI::MenuItemType >::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemType( Convert< MyGUI::MenuItemType >::From(_value) );
			}
		}



   	public:
		Convert< MyGUI::MenuCtrl * >::Type CreateItemChild( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert< MyGUI::MenuCtrl * >::To( static_cast<ThisType*>(mNative)->createItemChild( ) );
		}



   	public:
		Convert<size_t>::Type GetItemIndex( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<size_t>::To( static_cast<ThisType*>(mNative)->getItemIndex( ) );
		}



   	public:
		property Convert<const std::string &>::Type ItemId
		{
			Convert<const std::string &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const std::string &>::To( static_cast<ThisType*>(mNative)->getItemId() );
			}
			void set(Convert<const std::string &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemId( Convert<const std::string &>::From(_value) );
			}
		}



   	public:
		void RemoveItem( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->removeItem( );
		}



   	public:
		property Convert<const Ogre::UTFString &>::Type ItemName
		{
			Convert<const Ogre::UTFString &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const Ogre::UTFString &>::To( static_cast<ThisType*>(mNative)->getItemName() );
			}
			void set(Convert<const Ogre::UTFString &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setItemName( Convert<const Ogre::UTFString &>::From(_value) );
			}
		}



	};

} // namespace MMyGUI
