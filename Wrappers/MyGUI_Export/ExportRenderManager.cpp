/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "MyGUI_ExportRenderManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_CustomLayer.h"

namespace Export
{

	namespace ScopeRenderManager_GetBuffer
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportRenderManager_GetBuffer(
			Convert<size_t>::Type _index,
			Convert<void*&>::Type _buffer,
			Convert<size_t&>::Type _countVertex,
			Convert<size_t&>::Type _vertexId,
			Convert<size_t&>::Type _textureId,
			Convert<size_t&>::Type _layerId,
			Convert<bool&>::Type _change)
		{
			MyGUI::RenderBatchInfo* info = MyGUI::ExportRenderManager::getInstance().getBatchInfo(_index);
			if (info == nullptr)
				return false;

			_buffer = info->Buffer->lock();
			_countVertex = info->Count;
			_vertexId = info->Buffer->getId();
			_textureId = info->Texture->getId();
			_layerId = (size_t)info->Layer;
			_change = info->Buffer->getChange();

			return true;
		}
	}

	namespace ScopeRenderManager_AddTexture
	{
		MYGUIEXPORT void MYGUICALL ExportRenderManager_AddTexture(
			Convert<const std::string&>::Type _name,
			Convert<size_t>::Type _id,
			Convert<int>::Type _width,
			Convert<int>::Type _height)
		{
			MyGUI::ExportRenderManager::getInstance().addTexture(
				Convert<const std::string&>::From(_name),
				Convert<size_t>::From(_id),
				Convert<int>::From(_width),
				Convert<int>::From(_height));
		}
	}

	namespace ScopeRenderManager_GetLayer
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportRenderManager_GetLayer(
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
				//_name = Convert<const std::string&>::To("");
				return false;
			}

			MyGUI::ILayer* layer = MyGUI::LayerManager::getInstance().getLayer(index);
			MyGUI::CustomLayer* custom = layer->castType<MyGUI::CustomLayer>(false);

			if (custom == nullptr)
			{
				_layerId = 0;
				//_name = Convert<const std::string&>::To("");
				return true;
			}

			_layerId = (size_t)layer;
			_name = (void*)layer->getName().c_str();
			//_name = Convert<const std::string&>::To(layer->getName());
			return true;
		}
	}

}
