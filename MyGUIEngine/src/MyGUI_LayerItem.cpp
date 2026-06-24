/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_LayerItem.h"
#include <algorithm>

namespace MyGUI
{

	void LayerItem::addChildItem(LayerItem* _item)
	{
		mLayerItems.push_back(_item);
		if (mLayerNode != nullptr)
		{
			_item->attachToLayerItemNode(mLayerNode, false);
		}
	}

	void LayerItem::removeChildItem(LayerItem* _item)
	{
		VectorLayerItem::iterator item = std::remove(mLayerItems.begin(), mLayerItems.end(), _item);
		MYGUI_ASSERT(item != mLayerItems.end(), "item not found");
		mLayerItems.erase(item);
	}

	void LayerItem::addChildNode(LayerItem* _item)
	{
		mLayerNodes.push_back(_item);
		if (mLayerNode != nullptr)
		{
			// create new item for overlapped node
			ILayerNode* child_node = mLayerNode->createChildItemNode();
			_item->attachToLayerItemNode(child_node, true);
		}
	}

	void LayerItem::removeChildNode(LayerItem* _item)
	{
		VectorLayerItem::iterator item = std::remove(mLayerNodes.begin(), mLayerNodes.end(), _item);
		MYGUI_ASSERT(item != mLayerNodes.end(), "item not found");
		mLayerNodes.erase(item);
	}

	void LayerItem::addRenderItem(ISubWidget* _item)
	{
		mDrawItems.push_back(_item);
	}

	void LayerItem::removeAllRenderItems()
	{
		detachFromLayerItemNode(false);
		mDrawItems.clear();
	}

	void LayerItem::setRenderItemTexture(ITexture* _texture)
	{
		if (mTexture == _texture)
			return;

		mTexture = _texture;
		if (mLayerNode)
		{
			ILayerNode* node = mLayerNode;
			// later implement detach without text
			detachFromLayerItemNode(false);
			attachToLayerItemNode(node, false);
		}
	}

	void LayerItem::saveLayerItem()
	{
		mSaveLayerNode = mLayerNode;
	}

	void LayerItem::restoreLayerItem()
	{
		mLayerNode = mSaveLayerNode;
		if (mLayerNode)
		{
			attachToLayerItemNode(mLayerNode, false);
		}
	}

	void LayerItem::attachItemToNode(ILayer* _layer, ILayerNode* _node)
	{
		mLayer = _layer;
		mLayerNode = _node;

		attachToLayerItemNode(_node, true);
	}

	void LayerItem::detachFromLayer()
	{
		// we are already detached
		if (nullptr == mLayer)
			return;

		MYGUI_ASSERT(mLayerNode, "mLayerNode == nullptr");

		// unsubscribe from picking
		mLayerNode->detachLayerItem(this);

		// will be zeroed on detach
		ILayerNode* save = mLayerNode;

		detachFromLayerItemNode(true);

		// detach layer and zero at root widget
		mLayer->destroyChildItemNode(save);
		mLayerNode = nullptr;
		mLayer = nullptr;
	}

	void LayerItem::upLayerItem()
	{
		MyGUI::ILayerNode* node = mLayerNode;
		while (node)
		{
			node->getLayer()->upChildItemNode(node);
			node = node->getParent();
		}
	}

	void LayerItem::attachToLayerItemNode(ILayerNode* _item, bool _deep)
	{
		MYGUI_DEBUG_ASSERT(nullptr != _item, "attached item must be valid");

		// save so subsequent children can attach
		mLayerNode = _item;

		for (auto& drawItem : mDrawItems)
		{
			drawItem->createDrawItem(mTexture, _item);
		}

		for (auto& layerItem : mLayerItems)
		{
			layerItem->attachToLayerItemNode(_item, _deep);
		}

		for (auto& layerNode : mLayerNodes)
		{
			// create new item for overlapped node
			if (_deep)
			{
				ILayerNode* child_node = _item->createChildItemNode();
				layerNode->attachToLayerItemNode(child_node, _deep);
			}
		}
	}

	void LayerItem::detachFromLayerItemNode(bool _deep)
	{
		for (auto& layerItem : mLayerItems)
		{
			layerItem->detachFromLayerItemNode(_deep);
		}

		for (auto& layerNode : mLayerNodes)
		{
			if (_deep)
			{
				ILayerNode* node = layerNode->mLayerNode;
				layerNode->detachFromLayerItemNode(_deep);
				if (node)
				{
					node->getLayer()->destroyChildItemNode(node);
				}
			}
		}

		// we are already detached
		ILayerNode* node = mLayerNode;
		if (node)
		{
			for (auto& drawItem : mDrawItems)
			{
				drawItem->destroyDrawItem();
			}

			mLayerNode = nullptr;
		}
	}

	ILayer* LayerItem::getLayer() const
	{
		return mLayer;
	}

	ILayerNode* LayerItem::getLayerNode() const
	{
		return mLayerNode;
	}

} // namespace MyGUI
