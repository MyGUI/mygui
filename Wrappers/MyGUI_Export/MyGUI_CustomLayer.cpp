/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_CustomLayer.h"
#include "MyGUI_CustomLayerNode.h"
#include "MyGUI_ExportRenderManager.h"

namespace MyGUI
{

	ILayerNode* CustomLayer::createChildItemNode()
	{
		// создаем рутовый айтем
		ILayerNode* node = new CustomLayerNode(this);
		mChildItems.push_back(node);

		mOutOfDate = true;

		return node;
	}

	void CustomLayer::renderToTarget(IRenderTarget* _target, bool _update)
	{
		ExportRenderManager::getInstance().doRenderLayer(this);

		Base::renderToTarget(_target, _update);

		ExportRenderManager::getInstance().doRenderLayer(nullptr);
	}

}
