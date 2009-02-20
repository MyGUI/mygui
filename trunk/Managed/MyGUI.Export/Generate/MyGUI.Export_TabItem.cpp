/*!
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
