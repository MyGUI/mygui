﻿/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeMenuItemMethod_GetItemChild
	{
		MYGUIEXPORT Convert< MyGUI::MenuCtrl * >::Type MYGUICALL ExportMenuItem_GetItemChild( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::MenuCtrl * >::To( static_cast< MyGUI::MenuItem * >(_native)->getItemChild( ) );
		}
	}



   	namespace ScopeMenuItemMethod_GetMenuCtrlParent
	{
		MYGUIEXPORT Convert< MyGUI::MenuCtrl * >::Type MYGUICALL ExportMenuItem_GetMenuCtrlParent( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::MenuCtrl * >::To( static_cast< MyGUI::MenuItem * >(_native)->getMenuCtrlParent( ) );
		}
	}



   	namespace ScopeMenuItemMethod_SetItemChildVisible
	{
		MYGUIEXPORT void MYGUICALL ExportMenuItem_SetItemChildVisible_visible( MyGUI::Widget* _native ,
			Convert<bool>::Type _visible )
		{
			static_cast< MyGUI::MenuItem * >(_native)->setItemChildVisible(
				Convert<bool>::From(_visible) );
		}
	}



   	namespace ScopeMenuItemProperty_ItemType
	{
		MYGUIEXPORT Convert< MyGUI::MenuItemType >::Type MYGUICALL ExportMenuItem_GetItemType( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::MenuItemType >::To( static_cast< MyGUI::MenuItem * >(_native)->getItemType( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportMenuItem_SetItemType( MyGUI::Widget* _native , Convert< MyGUI::MenuItemType >::Type _value )
		{
			static_cast< MyGUI::MenuItem * >(_native)->setItemType( Convert< MyGUI::MenuItemType >::From( _value ) );
		}
	}
	


   	namespace ScopeMenuItemMethod_CreateItemChild
	{
		MYGUIEXPORT Convert< MyGUI::MenuCtrl * >::Type MYGUICALL ExportMenuItem_CreateItemChild( MyGUI::Widget* _native )
		{
			return Convert< MyGUI::MenuCtrl * >::To( static_cast< MyGUI::MenuItem * >(_native)->createItemChild( ) );
		}
	}



   	namespace ScopeMenuItemMethod_GetItemIndex
	{
		MYGUIEXPORT Convert<size_t>::Type MYGUICALL ExportMenuItem_GetItemIndex( MyGUI::Widget* _native )
		{
			return Convert<size_t>::To( static_cast< MyGUI::MenuItem * >(_native)->getItemIndex( ) );
		}
	}



   	namespace ScopeMenuItemProperty_ItemId
	{
		MYGUIEXPORT Convert<const std::string &>::Type MYGUICALL ExportMenuItem_GetItemId( MyGUI::Widget* _native )
		{
			return Convert<const std::string &>::To( static_cast< MyGUI::MenuItem * >(_native)->getItemId( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportMenuItem_SetItemId( MyGUI::Widget* _native , Convert<const std::string &>::Type _value )
		{
			static_cast< MyGUI::MenuItem * >(_native)->setItemId( Convert<const std::string &>::From( _value ) );
		}
	}
	


   	namespace ScopeMenuItemMethod_RemoveItem
	{
		MYGUIEXPORT void MYGUICALL ExportMenuItem_RemoveItem( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::MenuItem * >(_native)->removeItem( );
		}
	}



   	namespace ScopeMenuItemProperty_ItemName
	{
		MYGUIEXPORT Convert<const Ogre::UTFString &>::Type MYGUICALL ExportMenuItem_GetItemName( MyGUI::Widget* _native )
		{
			return Convert<const Ogre::UTFString &>::To( static_cast< MyGUI::MenuItem * >(_native)->getItemName( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportMenuItem_SetItemName( MyGUI::Widget* _native , Convert<const Ogre::UTFString &>::Type _value )
		{
			static_cast< MyGUI::MenuItem * >(_native)->setItemName( Convert<const Ogre::UTFString &>::From( _value ) );
		}
	}
	


}
