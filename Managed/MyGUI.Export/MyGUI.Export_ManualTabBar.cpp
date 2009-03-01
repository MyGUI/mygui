/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "ExportMarshalingWidget.h"
#include "ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

   	namespace ScopeTabBarMethod_GetItemDataAt
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportTabBar_GetItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			Convert<MyGUI::Any>::Type* data = static_cast< MyGUI::Tab * >(_native)->getItemDataAt< Convert<MyGUI::Any>::Type >(
				Convert<size_t>::From( _index ), false );
			return data == nullptr ? nullptr : *data;
		}
	}

	namespace ScopeTabBarMethod_GetItemData
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportTabBar_GetItemData_item( MyGUI::Widget* _native,
			Convert<MyGUI::TabItem *>::Type _item )
		{
			Convert<MyGUI::Any>::Type* data = static_cast< MyGUI::Tab * >(_native)->getItemData< Convert<MyGUI::Any>::Type >(
				Convert<MyGUI::TabItem *>::From( _item ), false );
			return data == nullptr ? nullptr : *data;
		}
	}

} // namespace Export
