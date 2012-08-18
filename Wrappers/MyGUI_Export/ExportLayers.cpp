/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_CustomLayer.h"
#include "MyGUI_ExportDiagnostic.h"
#include "MyGUI_CustomLayerNode.h"

namespace Export
{

	namespace ScopeLayers_GetLayer
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportLayers_GetLayer(
			Convert<size_t>::Type _index,
			Convert<size_t&>::Type _layerId,
			Convert<void*&>::Type _name)
		{
			_name = nullptr;
			size_t count = MyGUI::LayerManager::getInstance().getLayerCount();
			size_t index = Convert<size_t>::From(_index);
			if (index >= count)
			{
				_layerId = 0;
				return false;
			}

			MyGUI::ILayer* layer = MyGUI::LayerManager::getInstance().getLayer(index);
			MyGUI::CustomLayer* custom = layer->castType<MyGUI::CustomLayer>(false);

			if (custom == nullptr)
			{
				_layerId = 0;
				return true;
			}

			_layerId = (size_t)layer;
			_name = (void*)layer->getName().c_str();
			return true;
		}
	}

	namespace ScopeLayers_SetWidgetDepth
	{
		MYGUIEXPORT void MYGUICALL ExportLayers_SetWidgetDepth(
			MyGUI::Widget* _widget,
			Convert<float>::Type _depth)
		{
			MyGUI::ILayerNode* node = _widget->getLayerNode();
			if (node == nullptr)
			{
				MYGUI_PLATFORM_LOG(Warning, "Layer node not found");
				return;
			}

			MyGUI::CustomLayerNode* custom = node->castType<MyGUI::CustomLayerNode>(false);
			if (custom == nullptr)
			{
				MYGUI_PLATFORM_LOG(Warning, "Layer node is not not CustomLayerNode");
				return;
			}

			custom->setDepth(Convert<float>::From(_depth));
		}
	}

}
