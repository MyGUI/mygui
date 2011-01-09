/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef __EXPORT_WIDGET_MultiListItem_H__
#define __EXPORT_WIDGET_MultiListItem_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI.Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	//InsertPoint

   	namespace ScopeMultiListItemMethod_SetItemWidth
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListItem_SetItemWidth_value( MyGUI::Widget* _native,
			Convert<int>::Type _value )
		{
			static_cast< MyGUI::MultiListItem * >(_native)->setItemWidth(
				Convert<int>::From( _value ) );
		}
	}



   	namespace ScopeMultiListItemMethod_SetItemResizingPolicy
	{
		MYGUIEXPORT void MYGUICALL ExportMultiListItem_SetItemResizingPolicy_value( MyGUI::Widget* _native,
			Convert<MyGUI::ResizingPolicy>::Type _value )
		{
			static_cast< MyGUI::MultiListItem * >(_native)->setItemResizingPolicy(
				Convert<MyGUI::ResizingPolicy>::From( _value ) );
		}
	}



   


   


   


   


}

#endif // __EXPORT_WIDGET_MultiListItem_H__
