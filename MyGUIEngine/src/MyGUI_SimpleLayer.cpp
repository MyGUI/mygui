/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*//*
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
#include "MyGUI_Common.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_SimpleLayer.h"
#include "MyGUI_LayerNode.h"

namespace MyGUI
{

	SimpleLayer::SimpleLayer(const std::string& _name, bool _pick) :
		ILayer(_name),
		mIsPeek(_pick),
		mChildItem(nullptr)
	{
	}

	SimpleLayer::~SimpleLayer()
	{
		MYGUI_ASSERT(mChildItem == nullptr, "Layer '" << getName() << "' must be empty before destroy");
	}

	ILayerNode * SimpleLayer::createItemNode(ILayerNode* _parent)
	{
		// пусть парент сам рулит
		if (_parent)
		{
			return _parent->createItemNode();
		}

		if (mChildItem == nullptr)
		{
			mChildItem = new LayerNode(this);
		}

		mChildItem->_addUsing();
		return mChildItem;
	}

	void SimpleLayer::destroyItemNode(ILayerNode* _item)
	{
		LayerNode* item = static_cast<LayerNode*>(_item);
		LayerNode * parent = item->getParent();
		// если есть отец, то русть сам и удаляет
		if (parent)
		{
			parent->destroyItemNode(item);
		}
		// айтем рутовый, мы удаляем
		else
		{
			if (mChildItem == _item)
			{
				item->_removeUsing();
				if (0 == item->_countUsing())
				{
					delete item;
					mChildItem = nullptr;
				}
				return;
			}
			MYGUI_EXCEPT("item keeper not found");
		}
	}

	bool SimpleLayer::existItemNode(ILayerNode* _item)
	{
		LayerNode* item = static_cast<LayerNode*>(_item);
		if (mChildItem != nullptr)
		{
			if (mChildItem == _item || mChildItem->existItemNode(item)) return true;
		}
		return false;
	}

	void SimpleLayer::upItemNode(ILayerNode* _item)
	{
		LayerNode* item = static_cast<LayerNode*>(_item);
		LayerNode * parent = item->getParent();
		// если есть отец, то пусть сам рулит
		if (parent)
		{
			// возвращается рутовый айтем
			item = parent->upItemNode(item);
		}
	}

	ILayerItem * SimpleLayer::getLayerItemByPoint(int _left, int _top)
	{
		if (false == mIsPeek) return nullptr;
		if (mChildItem != nullptr)
		{
			ILayerItem * item = mChildItem->getLayerItemByPoint(_left, _top);
			if (item != nullptr) return item;
		}
		return nullptr;
	}

	void SimpleLayer::doRender(bool _update)
	{
		if (mChildItem != nullptr) mChildItem->_render(_update);
	}

	size_t SimpleLayer::getItemCount()
	{
		size_t count = 0;
		if (mChildItem != nullptr)
		{
			count += mChildItem->getItemCount();
		}
		return count;
	}

	size_t SimpleLayer::getSubItemCount()
	{
		size_t count = 0;
		if (mChildItem != nullptr)
		{
			count += mChildItem->getItemCount() - 1;
		}
		return count;
	}

} // namespace MyGUI
