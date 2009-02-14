
#include "ExportDefine.h"
#include <MyGUI.h>

namespace Export
{

	MYGUIEXPORT void MYGUICALL ExportWidget_GetCaption(MyGUI::Widget* _widget, LPWStr & _caption)
	{
		_caption = _widget->getCaption().asWStr_c_str();
	}

	MYGUIEXPORT void MYGUICALL ExportWidget_SetCaption(MyGUI::Widget* _widget, LPWStr _caption)
	{
		_widget->setCaption(_caption);
	}

}