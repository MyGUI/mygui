/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_PopupMenu_H__
#define __EXPORT_WIDGET_PopupMenu_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

	namespace ScopePopupMenuProperty_Type
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportPopupMenu_IsType( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::PopupMenu * >(_native)->isType( ) );
		}
	}
	
}

#endif // __EXPORT_WIDGET_PopupMenu_H__
