/*!
	@file
	@author		Albert Semenov
	@date		02/2008
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
#include "MyGUI_Common.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_OverlappedLayer.h"
#include "MyGUI_LayerNode.h"

namespace MyGUI
{

	OverlappedLayer::OverlappedLayer(const std::string& _name, bool _pick) :
		ILayer(_name),
		mIsPick(_pick)
	{
	}

	OverlappedLayer::~OverlappedLayer()
	{
		MYGUI_ASSERT(mChildItems.empty(), "Layer '" << getName() << "' must be empty before destroy");
	}

	ILayerNode * OverlappedLayer::createChildItemNode()
	{
		// ������� ������� �����
		ILayerNode * node = new LayerNode(this);
		mChildItems.push_back(node);

		return node;
	}

	void OverlappedLayer::destroyChildItemNode(ILayerNode* _item)
	{
		// ���� ���� ����, �� ����� ��� � �������
		ILayerNode * parent = _item->getParent();
		if (parent)
		{
			parent->destroyChildItemNode(_item);
			return;
		}

		// ����� �������, �� �������
		for (VectorILayerNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			if ((*iter) == _item)
			{
				delete _item;
				mChildItems.erase(iter);
				return;
			}
		}
		MYGUI_EXCEPT("item node not found");
	}

	void OverlappedLayer::upChildItemNode(ILayerNode* _item)
	{
		// ���� ���� ����, �� ����� ��� �����
		ILayerNode* parent = _item->getParent();
		if (parent != nullptr)
		{
			parent->upChildItemNode(_item);
			return;
		}

		if ((2 > mChildItems.size()) || (mChildItems.back() == _item)) return;
		for (VectorILayerNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			if ((*iter) == _item)
			{
				mChildItems.erase(iter);
				mChildItems.push_back(_item);
				return;
			}
		}

		MYGUI_EXCEPT("item node not found");
	}

	ILayerItem * OverlappedLayer::getLayerItemByPoint(int _left, int _top)
	{
		if (false == mIsPick) return nullptr;
		VectorILayerNode::reverse_iterator iter = mChildItems.rbegin();
		while (iter != mChildItems.rend())
		{
			ILayerItem * item = (*iter)->getLayerItemByPoint(_left, _top);
			if (item != nullptr) return item;
			++iter;
		}
		return nullptr;
	}

	void OverlappedLayer::renderToTarget(IRenderTarget* _target, bool _update)
	{
		for (VectorILayerNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			(*iter)->renderToTarget(_target, _update);
		}
	}

	EnumeratorILayerNode OverlappedLayer::getEnumerator()
	{
		return EnumeratorILayerNode(mChildItems);
	}

} // namespace MyGUI
