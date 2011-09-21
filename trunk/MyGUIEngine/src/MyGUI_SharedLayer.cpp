/*!
	@file
	@author		Albert Semenov
	@date		02/2008
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
#include "MyGUI_SharedLayer.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_RenderManager.h"

namespace MyGUI
{

	SharedLayer::SharedLayer() :
		mIsPick(false),
		mChildItem(nullptr),
		mOutOfDate(false)
	{
		mViewSize = RenderManager::getInstance().getViewSize();
	}

	SharedLayer::~SharedLayer()
	{
		MYGUI_ASSERT(mChildItem == nullptr, "Layer '" << getName() << "' must be empty before destroy");
	}

	void SharedLayer::deserialization(xml::ElementPtr _node, Version _version)
	{
		mName = _node->findAttribute("name");
		if (_version >= Version(1, 2))
		{
			MyGUI::xml::ElementEnumerator propert = _node->getElementEnumerator();
			while (propert.next("Property"))
			{
				const std::string& key = propert->findAttribute("key");
				const std::string& value = propert->findAttribute("value");
				if (key == "Pick")
					mIsPick = utility::parseValue<bool>(value);
			}
		}
		else if (_version >= Version(1, 0))
		{
			mIsPick = utility::parseBool(_node->findAttribute("pick"));
		}
		else
		{
			mIsPick = utility::parseBool(_node->findAttribute("peek"));
		}
	}

	ILayerNode* SharedLayer::createChildItemNode()
	{
		if (mChildItem == nullptr)
			mChildItem = new SharedLayerNode(this);

		mChildItem->addUsing();

		mOutOfDate = true;

		return mChildItem;
	}

	void SharedLayer::destroyChildItemNode(ILayerNode* _item)
	{
		// айтем рутовый, мы удаляем
		if (mChildItem == _item)
		{
			mChildItem->removeUsing();
			if (0 == mChildItem->countUsing())
			{
				delete mChildItem;
				mChildItem = nullptr;
			}

			mOutOfDate = true;

			return;
		}
		//MYGUI_EXCEPT("item node not found");
	}

	void SharedLayer::upChildItemNode(ILayerNode* _item)
	{
		// если есть отец, то пусть сам рулит
		ILayerNode* parent = _item->getParent();
		if (parent != nullptr)
			parent->upChildItemNode(_item);

		mOutOfDate = true;
	}

	ILayerItem* SharedLayer::getLayerItemByPoint(int _left, int _top) const
	{
		if (!mIsPick)
			return nullptr;

		if (mChildItem != nullptr)
		{
			ILayerItem* item = mChildItem->getLayerItemByPoint(_left, _top);
			if (item != nullptr)
				return item;
		}
		return nullptr;
	}

	IntPoint SharedLayer::getPosition(int _left, int _top) const
	{
		return IntPoint(_left, _top);
	}

	void SharedLayer::renderToTarget(IRenderTarget* _target, bool _update)
	{
		if (mChildItem != nullptr)
			mChildItem->renderToTarget(_target, _update);

		mOutOfDate = false;
	}

	void SharedLayer::resizeView(const IntSize& _viewSize)
	{
		if (mChildItem != nullptr)
			mChildItem->resizeView(_viewSize);

		mViewSize = _viewSize;
	}

	EnumeratorILayerNode SharedLayer::getEnumerator() const
	{
		static VectorILayerNode nodes;
		if (mChildItem == nullptr)
		{
			nodes.clear();
		}
		else
		{
			if (nodes.empty())
				nodes.push_back(mChildItem);
			else
				nodes[0] = mChildItem;
		}

		return EnumeratorILayerNode(nodes);
	}

	const IntSize& SharedLayer::getSize() const
	{
		return mViewSize;
	}

	bool SharedLayer::isOutOfDate() const
	{
		if (mChildItem->isOutOfDate())
			return true;

		return mOutOfDate;
	}

} // namespace MyGUI
