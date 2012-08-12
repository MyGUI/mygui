/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "ExportDefine.h"
#include "ExportMarshaling.h"
#include "MyGUI_ExportRenderManager.h"

namespace Export
{

	namespace ScopeRenderManager_GetBuffer
	{
		MYGUIEXPORT Convert<bool>::Type MYGUICALL ExportRenderManager_GetBuffer(
			Convert<size_t>::Type _index,
			Convert<void*&>::Type _buffer,
			Convert<size_t&>::Type _countVertex,
			Convert<size_t&>::Type _id,
			Convert<bool&>::Type _change)
		{
			MyGUI::RenderBatchInfo* info = MyGUI::ExportRenderManager::getInstance().getBatchInfo(_index);
			if (info == nullptr)
				return false;

			_buffer = info->Buffer->lock();
			_countVertex = info->Buffer->getVertexCount();
			_id = info->Buffer->getId();
			_change = info->Buffer->getChange();

			return true;
		}
	}

}
