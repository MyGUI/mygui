/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/

#include "LayerNode.h"

namespace MyGUI
{

	ILayerNode* LayerNode::createItemNode()
	{
		return 0;
	}

	void LayerNode::destroyItemNode()
	{
	}

	void LayerNode::attachLayerItem(ILayerItem* _item)
	{
		_item->attachItemToNode(mLayer, this);
		mItems.push_back(_item);
	}

	void LayerNode::detachLayerItem(ILayerItem* _root)
	{
	}

	void LayerNode::upItemNode()
	{
	}

	IRenderItem* LayerNode::addToRenderItem(const std::string& _texture, IDrawItem* _item)
	{
		// ищем айтем с такой же текстурой
		RenderItem* item = 0;
		for (size_t index=0; index<mRenderItem.size(); ++index)
		{
			if (mRenderItem[index]->getTexture() == _texture)
			{
				item = mRenderItem[index];
				break;
			}
		}

		// ненайденно, создаем
		if (item == 0)
		{
			item = new RenderItem(_texture);
			mRenderItem.push_back(item);
		}

		return item;
	}

	void LayerNode::doRender(bool _update)
	{
		// обновляем все айтемы
		for (size_t index=0; index<mRenderItem.size(); ++index)
		{
			mRenderItem[index]->doRender(_update);
		}
	}

} // namespace MyGUI
