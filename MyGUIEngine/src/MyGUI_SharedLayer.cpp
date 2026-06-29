/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_SharedLayer.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_RenderManager.h"

#include <pugixml.hpp>

namespace MyGUI
{

	SharedLayer::SharedLayer()
	{
		mViewSize = RenderManager::getInstance().getViewSize();
	}

	SharedLayer::~SharedLayer()
	{
		if (mChildItem != nullptr)
			MYGUI_LOG(Critical, "Layer '" << getName() << "' must be empty before destroy");
	}

	void SharedLayer::deserialization(xml::ElementPtr _node, Version _version)
	{
		mName = _node.node().attribute("name").value();
		if (_version >= Version(1, 2))
		{
			for (auto propert : _node.node().children("Property"))
			{
				std::string_view key = propert.attribute("key").value();
				std::string_view value = propert.attribute("value").value();
				if (key == "Pick")
					setPick(utility::parseValue<bool>(value));
			}
		}
		else if (_version >= Version(1, 0))
		{
			setPick(utility::parseBool(_node.node().attribute("pick").value()));
		}
		else
		{
			setPick(utility::parseBool(_node.node().attribute("peek").value()));
		}
	}

	void SharedLayer::setPick(bool _pick)
	{
		mIsPick = _pick;
	}

	ILayerNode* SharedLayer::createChildItemNode()
	{
		if (mChildItem == nullptr)
			mChildItem = new SharedLayerNode(this);

		mChildItem->addUsing();

		mOutOfDate = true;

		mChildItems.clear();
		mChildItems.push_back(mChildItem);

		return mChildItem;
	}

	void SharedLayer::destroyChildItemNode(ILayerNode* _item)
	{
		// item is root, we delete
		if (mChildItem == _item)
		{
			mChildItem->removeUsing();
			if (0 == mChildItem->countUsing())
			{
				delete mChildItem;
				mChildItem = nullptr;
			}

			mOutOfDate = true;

			mChildItems.clear();

			return;
		}
		//MYGUI_EXCEPT("item node not found");
	}

	void SharedLayer::upChildItemNode(ILayerNode* _item)
	{
		// if has a parent, let it manage itself
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
		return {_left, _top};
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
		return EnumeratorILayerNode(mChildItems);
	}

	const VectorILayerNode& SharedLayer::getChildItems() const
	{
		return mChildItems;
	}

	size_t SharedLayer::getLayerNodeCount() const
	{
		return mChildItem == nullptr ? 0 : 1;
	}

	ILayerNode* SharedLayer::getLayerNodeAt(size_t _index) const
	{
		size_t count = getLayerNodeCount();
		MYGUI_ASSERT_RANGE(_index, count, "SharedLayer::getLayerNodeAt");

		return mChildItem;
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
