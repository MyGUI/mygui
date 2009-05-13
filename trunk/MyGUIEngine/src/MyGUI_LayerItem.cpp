/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_LayerItem.h"

namespace MyGUI
{

	void LayerItem::addChildItem(LayerItem* _item)
	{
		mLayerItems.push_back(_item);
		if (mLayerNode != nullptr)
		{
			_item->attachToLayerItemKeeper(mLayerNode, false);
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
	}

	void LayerItem::removeChildNode(LayerItem* _item)
	{
		VectorLayerItem::iterator item = std::remove(mLayerNodes.begin(), mLayerNodes.end(), _item);
		MYGUI_ASSERT(item != mLayerNodes.end(), "item not found");
		mLayerNodes.erase(item);
	}

	void LayerItem::addRenderItem(IDrawItem* _item)
	{
		mDrawItems.push_back(_item);
	}

	void LayerItem::removeAllRenderItems()
	{
		detachFromLayerItemKeeper(false);
		mDrawItems.clear();
	}

	void LayerItem::setRenderItemTexture(const std::string& _texture)
	{
		mTexture = _texture;
		if (mLayerNode)
		{
			ILayerNode* node = mLayerNode;
			// позже сделать детач без текста
			detachFromLayerItemKeeper(false);
			attachToLayerItemKeeper(node, false);
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
			attachToLayerItemKeeper(mLayerNode, false);
		}
	}

	void LayerItem::attachItemToNode(ILayer* _layer, ILayerNode* _node)
	{
		mLayer = _layer;
		mLayerNode = _node;

		attachToLayerItemKeeper(_node, true);
	}

	void LayerItem::detachFromLayer()
	{
		// мы уже отдетачены в доску
		if (nullptr == mLayer) return;

		// такого быть не должно
		MYGUI_ASSERT(mLayerNode, "_item->mLayerNode == nullptr");

		// отписываемся от пиккинга
		mLayerNode->detachLayerItem(this);

		// при детаче обнулиться
		ILayerNode * save = mLayerNode;

		// физически отсоединяем
		detachFromLayerItemKeeper(true);

		// отсоединяем леер и обнуляем у рутового виджета
		mLayer->destroyItemNode(save);
		mLayerNode = nullptr;
		mLayer = nullptr;
	}

	void LayerItem::upLayerItem()
	{
		if (mLayerNode) mLayerNode->upItemNode();
	}

	void LayerItem::attachToLayerItemKeeper(ILayerNode* _item, bool _deep)
	{
		MYGUI_DEBUG_ASSERT(nullptr != _item, "attached item must be valid");

		// сохраняем, чтобы последующие дети могли приаттачиться
		mLayerNode = _item;

		for (VectorIDrawItem::iterator skin = mDrawItems.begin(); skin != mDrawItems.end(); ++skin)
		{
			(*skin)->createDrawItem(mTexture, _item);
		}

		for (VectorLayerItem::iterator item = mLayerItems.begin(); item != mLayerItems.end(); ++item)
		{
			(*item)->attachToLayerItemKeeper(_item, _deep);
		}
	}

	void LayerItem::detachFromLayerItemKeeper(bool _deep)
	{

		for (VectorLayerItem::iterator item = mLayerItems.begin(); item != mLayerItems.end(); ++item)
		{
			(*item)->detachFromLayerItemKeeper(_deep);
		}

		// мы уже отаттачены
		ILayerNode* node = mLayerNode;
		if (node)
		{

			//for (VectorWidgetPtr::iterator widget = mWidgetChildSkin.begin(); widget != mWidgetChildSkin.end(); ++widget) (*widget)->_detachFromLayerItemKeeperByStyle(_deep);
			for (VectorIDrawItem::iterator skin = mDrawItems.begin(); skin != mDrawItems.end(); ++skin)
			{
				(*skin)->destroyDrawItem();
			}

			// при глубокой очистке, если мы оверлаппед, то для нас создавали айтем
			/*if (_deep && !this->isRootWidget() && mWidgetStyle == WidgetStyle::Overlapped)
			{
				node->destroyItemNode();
			}*/
			// очищаем
			mLayerNode = nullptr;
		}
	}

} // namespace MyGUI
