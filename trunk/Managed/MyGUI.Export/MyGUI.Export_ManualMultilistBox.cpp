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

   	namespace ScopeMultiListBoxMethod_GetItemDataAt
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportMultiListBox_GetItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			Convert<MyGUI::Any>::Type* data = static_cast< MyGUI::MultiList * >(_native)->getItemDataAt< Convert<MyGUI::Any>::Type >(
				Convert<size_t>::From( _index ), false );
			return data == nullptr ? nullptr : *data;
		}
	}

   	namespace ScopeMultiListBoxMethod_GetColumnDataAt
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportMultiListBox_GetColumnDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _index )
		{
			Convert<MyGUI::Any>::Type* data = static_cast< MyGUI::MultiList * >(_native)->getColumnDataAt< Convert<MyGUI::Any>::Type >(
				Convert<size_t>::From( _index ), false );
			return data == nullptr ? nullptr : *data;
		}
	}

   	namespace ScopeMultiListBoxMethod_GetSubItemDataAt
	{
		MYGUIEXPORT Convert<MyGUI::Any>::Type MYGUICALL ExportMultiListBox_GetSubItemDataAt_index( MyGUI::Widget* _native,
			Convert<size_t>::Type _column ,
			Convert<size_t>::Type _index )
		{
			Convert<MyGUI::Any>::Type* data = static_cast< MyGUI::MultiList * >(_native)->getSubItemDataAt< Convert<MyGUI::Any>::Type >(
				Convert<size_t>::From( _column ), Convert<size_t>::From( _index ), false );
			return data == nullptr ? nullptr : *data;
		}
	}

} // namespace Export
