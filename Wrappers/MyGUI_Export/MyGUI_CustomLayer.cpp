/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_Precompiled.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_CustomLayer.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_ExportRenderManager.h"

namespace MyGUI
{

	CustomLayer::CustomLayer() :
		mIsPick(false),
		mChildItem(nullptr),
		mOutOfDate(false)
	{
		mViewSize = RenderManager::getInstance().getViewSize();
	}

	CustomLayer::~CustomLayer()
	{
		MYGUI_ASSERT(mChildItem == nullptr, "Layer '" << getName() << "' must be empty before destroy");
	}

	void CustomLayer::deserialization(xml::ElementPtr _node, Version _version)
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

	ILayerNode* CustomLayer::createChildItemNode()
	{
		if (mChildItem == nullptr)
			mChildItem = new SharedLayerNode(this);

		mChildItem->addUsing();

		mOutOfDate = true;

		return mChildItem;
	}

	void CustomLayer::destroyChildItemNode(ILayerNode* _item)
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

	void CustomLayer::upChildItemNode(ILayerNode* _item)
	{
		// если есть отец, то пусть сам рулит
		ILayerNode* parent = _item->getParent();
		if (parent != nullptr)
			parent->upChildItemNode(_item);

		mOutOfDate = true;
	}

	ILayerItem* CustomLayer::getLayerItemByPoint(int _left, int _top) const
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

	IntPoint CustomLayer::getPosition(int _left, int _top) const
	{
		return IntPoint(_left, _top);
	}

	void CustomLayer::renderToTarget(IRenderTarget* _target, bool _update)
	{
		if (mChildItem != nullptr)
		{
			ExportRenderManager::getInstance().doRenderLayer(this);
			mChildItem->renderToTarget(_target, _update);
			ExportRenderManager::getInstance().doRenderLayer(nullptr);
		}

		mOutOfDate = false;
	}

	void CustomLayer::resizeView(const IntSize& _viewSize)
	{
		if (mChildItem != nullptr)
			mChildItem->resizeView(_viewSize);

		mViewSize = _viewSize;
	}

	EnumeratorILayerNode CustomLayer::getEnumerator() const
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

	const IntSize& CustomLayer::getSize() const
	{
		return mViewSize;
	}

	bool CustomLayer::isOutOfDate() const
	{
		if (mChildItem->isOutOfDate())
			return true;

		return mOutOfDate;
	}

}
