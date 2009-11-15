/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_TabItem_H__
#define __EXPORT_WIDGET_TabItem_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeTabItemMethod_RemoveItem
	{
		MYGUIEXPORT void MYGUICALL ExportTabItem_RemoveItem( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::TabItem * >(_native)->removeItem( );
		}
	}



   	namespace ScopeTabItemMethod_SetItemSelected
	{
		MYGUIEXPORT void MYGUICALL ExportTabItem_SetItemSelected( MyGUI::Widget* _native )
		{
			static_cast< MyGUI::TabItem * >(_native)->setItemSelected( );
		}
	}



   	namespace ScopeTabItemProperty_ItemData
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportTabItem_GetItemData( MyGUI::Widget* _native )
		{
			Convert<MyGUI::Any>::Type* data = static_cast< MyGUI::TabItem * >(_native)->getItemData<Convert<MyGUI::Any>::Type>( false );
			return data == nullptr ? nullptr : *data;
		}
		MYGUIEXPORT void MYGUICALL ExportTabItem_SetItemData( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::TabItem * >(_native)->setItemData( Convert<bool>::From( _value ) );
		}
	}



   


   	namespace ScopeTabItemProperty_ItemName
	{
		MYGUIEXPORT Convert<const Ogre::UTFString &>::Type MYGUICALL ExportTabItem_GetItemName( MyGUI::Widget* _native )
		{
			return Convert<const Ogre::UTFString &>::To( static_cast< MyGUI::TabItem * >(_native)->getItemName( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportTabItem_SetItemName( MyGUI::Widget* _native , Convert<const Ogre::UTFString &>::Type _value )
		{
			static_cast< MyGUI::TabItem * >(_native)->setItemName( Convert<const Ogre::UTFString &>::From( _value ) );
		}
	}



   	namespace ScopeTabItemProperty_ButtonWidth
	{
		MYGUIEXPORT Convert<int>::Type MYGUICALL ExportTabItem_GetButtonWidth( MyGUI::Widget* _native )
		{
			return Convert<int>::To( static_cast< MyGUI::TabItem * >(_native)->getButtonWidth( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportTabItem_SetButtonWidth( MyGUI::Widget* _native , Convert<int>::Type _value )
		{
			static_cast< MyGUI::TabItem * >(_native)->setButtonWidth( Convert<int>::From( _value ) );
		}
	}



   


   


   


}

#endif // __EXPORT_WIDGET_TabItem_H__
