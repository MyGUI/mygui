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

	namespace ScopeTabItemProperty_ItemData
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportTabItem_GetItemData( MyGUI::Widget* _native )
		{
			Convert<MyGUI::Any>::Type* data = static_cast< MyGUI::TabItem * >(_native)->getItemData< Convert<MyGUI::Any>::Type >( false );
			return data == nullptr ? nullptr : *data;
		}
		MYGUIEXPORT void MYGUICALL ExportTabItem_SetItemData( MyGUI::Widget* _native , Convert<MyGUI::Any>::Type _value )
		{
			static_cast< MyGUI::TabItem * >(_native)->setItemData( Convert<MyGUI::Any>::From( _value ) );
		}
	}

} // namespace Export
