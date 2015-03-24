/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

#ifndef EXPORT_WIDGET_TabItem_H_
#define EXPORT_WIDGET_TabItem_H_

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"
#include "MyGUI_Export_MarshalingWidget.h"
#include "../ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{
	//InsertPoint

	namespace ScopeTabItemMethod_SetButtonWidth
	{
		MYGUIEXPORT void MYGUICALL ExportTabItem_SetButtonWidth__value(MyGUI::Widget* _native,
			Convert<int>::Type _value)
		{
			static_cast<MyGUI::TabItem*>(_native)->setButtonWidth(
				Convert<int>::From(_value));
		}
	}
}

#endif
