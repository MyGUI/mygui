/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_OverlappedLayer.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_RenderManager.h"

#include <pugixml.hpp>

namespace MyGUI
{

	OverlappedLayer::OverlappedLayer()
	{
		mViewSize = RenderManager::getInstance().getViewSize();
	}

	OverlappedLayer::~OverlappedLayer()
	{
		if (!mChildItems.empty())
			MYGUI_LOG(Critical, "Layer '" << getName() << "' must be empty before destroy");
	}

	void OverlappedLayer::deserialization(xml::ElementPtr _node, Version _version)
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

	void OverlappedLayer::setPick(bool _pick)
	{
		mIsPick = _pick;
	}

	ILayerNode* OverlappedLayer::createChildItemNode()
	{
		ILayerNode* node = new LayerNode(this);
		mChildItems.push_back(node);

		mOutOfDate = true;

		return node;
	}

	void OverlappedLayer::destroyChildItemNode(ILayerNode* _item)
	{
		// if has parent, let parent handle deletion
		ILayerNode* parent = _item->getParent();
		if (parent)
		{
			parent->destroyChildItemNode(_item);

			mOutOfDate = true;

			return;
		}

		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
		{
			if ((*iter) == _item)
			{
				delete _item;
				mChildItems.erase(iter);

				mOutOfDate = true;

				return;
			}
		}

		MYGUI_EXCEPT("item node not found");
	}

	void OverlappedLayer::upChildItemNode(ILayerNode* _item)
	{
		// if has parent, let parent handle
		ILayerNode* parent = _item->getParent();
		if (parent != nullptr)
		{
			parent->upChildItemNode(_item);

			mOutOfDate = true;

			return;
		}

		if ((2 > mChildItems.size()) || (mChildItems.back() == _item))
			return;

		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
		{
			if ((*iter) == _item)
			{
				mChildItems.erase(iter);
				mChildItems.push_back(_item);

				mOutOfDate = true;

				return;
			}
		}

		MYGUI_EXCEPT("item node not found");
	}

	ILayerItem* OverlappedLayer::getLayerItemByPoint(int _left, int _top) const
	{
		if (!mIsPick)
			return nullptr;

		VectorILayerNode::const_reverse_iterator iter = mChildItems.rbegin();
		while (iter != mChildItems.rend())
		{
			ILayerItem* item = (*iter)->getLayerItemByPoint(_left, _top);
			if (item != nullptr)
				return item;
			++iter;
		}
		return nullptr;
	}

	IntPoint OverlappedLayer::getPosition(int _left, int _top) const
	{
		return {_left, _top};
	}

	void OverlappedLayer::renderToTarget(IRenderTarget* _target, bool _update)
	{
		for (auto& childItem : mChildItems)
			childItem->renderToTarget(_target, _update);

		mOutOfDate = false;
	}

	void OverlappedLayer::resizeView(const IntSize& _viewSize)
	{
		for (auto& childItem : mChildItems)
			childItem->resizeView(_viewSize);

		mViewSize = _viewSize;
	}

	EnumeratorILayerNode OverlappedLayer::getEnumerator() const
	{
		return EnumeratorILayerNode(mChildItems);
	}

	const VectorILayerNode& OverlappedLayer::getChildItems() const
	{
		return mChildItems;
	}

	size_t OverlappedLayer::getLayerNodeCount() const
	{
		return mChildItems.size();
	}

	ILayerNode* OverlappedLayer::getLayerNodeAt(size_t _index) const
	{
		MYGUI_ASSERT_RANGE(_index, mChildItems.size(), "OverlappedLayer::getLayerNodeAt");

		return mChildItems[_index];
	}

	const IntSize& OverlappedLayer::getSize() const
	{
		return mViewSize;
	}

	bool OverlappedLayer::isOutOfDate() const
	{
		for (const auto& childItem : mChildItems)
		{
			if (static_cast<const LayerNode*>(childItem)->isOutOfDate())
				return true;
		}

		return mOutOfDate;
	}

} // namespace MyGUI
