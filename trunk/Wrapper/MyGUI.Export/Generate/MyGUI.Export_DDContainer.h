/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_DDContainer_H__
#define __EXPORT_WIDGET_DDContainer_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint















   


   	namespace ScopeDDContainerProperty_NeedDragDrop
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportDDContainer_GetNeedDragDrop( MyGUI::Widget* _native )
		{
			return Convert<bool>::To( static_cast< MyGUI::DDContainer * >(_native)->getNeedDragDrop( ) );
		}
		MYGUIEXPORT void MYGUICALL ExportDDContainer_SetNeedDragDrop( MyGUI::Widget* _native , Convert<bool>::Type _value )
		{
			static_cast< MyGUI::DDContainer * >(_native)->setNeedDragDrop( Convert<bool>::From( _value ) );
		}
	}



   


   


   


}

#endif // __EXPORT_WIDGET_DDContainer_H__
