/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/

#include "DefaultLayer.h"
#include <assert.h>

namespace MyGUI
{

	ILayerNode* DefaultLayer::createItemNode(ILayerNode* _parent)
	{
		LayerNode* node = new LayerNode(this);
		mNodes.push_back(node);
		return node;
	}

	void DefaultLayer::destroyItemNode(ILayerNode* _item)
	{
		for (size_t index=0; index<mNodes.size(); ++index)
		{
			if (mNodes[index] == _item)
			{
				mNodes.erase(mNodes.begin() + index);
				return;
			}
		}
		assert(0);
	}

	bool DefaultLayer::existItemNode(ILayerNode* _item)
	{
		return false;
	}

	void DefaultLayer::upItemNode(ILayerNode* _item)
	{
	}

	void DefaultLayer::doRender(bool _update)
	{
		for (size_t index=0; index<mNodes.size(); ++index)
		{
			mNodes[index]->doRender(_update);
		}
	}

	ILayerItem* DefaultLayer::getLayerItemByPoint(int _left, int _top)
	{
		return 0;
	}

} // namespace MyGUI
