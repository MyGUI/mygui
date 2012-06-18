
#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "Generate/MyGUI_Export_MarshalingWidget.h"
#include "ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	MYGUIEXPORT bool MYGUICALL ExportPointerManager_GetVisible( )
	{
		return MyGUI::PointerManager::getInstance().isVisible();
	}

	MYGUIEXPORT void MYGUICALL ExportPointerManager_SetVisible( bool _value )
	{
		MyGUI::PointerManager::getInstance().setVisible(_value);
	}

}
