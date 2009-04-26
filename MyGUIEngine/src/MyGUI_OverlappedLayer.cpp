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
#include "MyGUI_OverlappedLayer.h"
#include "MyGUI_LayerNode.h"

namespace MyGUI
{

	OverlappedLayer::OverlappedLayer(const std::string& _name, bool _pick) :
		ILayer(_name),
		mIsPeek(_pick)
	{
	}

	OverlappedLayer::~OverlappedLayer()
	{
		MYGUI_ASSERT(mChildItems.empty(), "Layer '" << getName() << "' must be empty before destroy");
	}

	ILayerNode * OverlappedLayer::createItemNode(ILayerNode* _parent)
	{
		// пусть парент сам рулит
		if (_parent)
		{
			return _parent->createItemNode();
		}

		// создаем рутовый айтем
		LayerNode * layer = new LayerNode(this);
		mChildItems.push_back(layer);

		layer->_addUsing();
		return layer;
	}

	void OverlappedLayer::destroyItemNode(ILayerNode* _item)
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
			for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
			{
				if ((*iter) == _item)
				{
					item->_removeUsing();
					if (0 == item->_countUsing())
					{
						delete item;
						mChildItems.erase(iter);
					}
					return;
				}
			}
			MYGUI_EXCEPT("item keeper not found");
		}
	}

	bool OverlappedLayer::existItemNode(ILayerNode* _item)
	{
		LayerNode* item = static_cast<LayerNode*>(_item);
		for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			if ((*iter) == _item || (*iter)->existItemNode(item)) return true;
		}
		return false;
	}

	void OverlappedLayer::upItemNode(ILayerNode* _item)
	{
		LayerNode* item = static_cast<LayerNode*>(_item);
		LayerNode * parent = item->getParent();
		// если есть отец, то пусть сам рулит
		if (parent)
		{
			// возвращается рутовый айтем
			item = parent->upItemNode(item);
		}

		if ((2 > mChildItems.size()) || (mChildItems.back() == _item)) return;
		for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			if ((*iter) == item)
			{
				mChildItems.erase(iter);
				mChildItems.push_back(item);
				return;
			}
		}

		MYGUI_EXCEPT("item keeper not found");
	}

	ILayerItem * OverlappedLayer::getLayerItemByPoint(int _left, int _top)
	{
		if (false == mIsPeek) return nullptr;
		VectorLayerItemKeeper::reverse_iterator iter = mChildItems.rbegin();
		while (iter != mChildItems.rend())
		{
			ILayerItem * item = (*iter)->getLayerItemByPoint(_left, _top);
			if (item != nullptr) return item;
			++iter;
		}
		return nullptr;
	}

	void OverlappedLayer::doRender(bool _update)
	{
		for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			(*iter)->_render(_update);
		}
	}

	size_t OverlappedLayer::getItemCount()
	{
		size_t count = 0;
		for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			count += (*iter)->getItemCount();
		}
		return count;
	}

	size_t OverlappedLayer::getSubItemCount()
	{
		size_t count = 0;
		for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			count += (*iter)->getItemCount();
		}
		return count - mChildItems.size();
	}

} // namespace MyGUI
