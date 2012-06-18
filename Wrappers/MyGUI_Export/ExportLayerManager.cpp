
#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "Generate/MyGUI_Export_MarshalingWidget.h"
#include "ExportMarshalingType.h"
#include <MyGUI.h>

namespace Export
{

	MYGUIEXPORT void MYGUICALL ExportLayerManager_AttachToLayer(
		Convert<const std::string&>::Type _layer ,
		MyGUI::Widget* _widget )
	{
		MyGUI::LayerManager::getInstance().attachToLayerNode(
			Convert<const std::string&>::From(_layer) ,
			_widget );
	}

	MYGUIEXPORT void MYGUICALL ExportLayerManager_UpWidget(
		MyGUI::Widget* _widget )
	{
		MyGUI::LayerManager::getInstance().upLayerItem(
			_widget );
	}

}
