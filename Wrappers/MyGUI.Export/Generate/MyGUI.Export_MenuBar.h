/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_MenuBar_H__
#define __EXPORT_WIDGET_MenuBar_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

	namespace ScopeMenuBarProperty_Type
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportMenuBar_IsType( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::MenuBar * >(_native)->isType( ) );
		}
	}
	
}

#endif // __EXPORT_WIDGET_MenuBar_H__
