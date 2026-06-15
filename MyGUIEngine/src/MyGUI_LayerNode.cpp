/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_ILayerItem.h"
#include "MyGUI_ITexture.h"
#include "MyGUI_ISubWidget.h"
#include "MyGUI_ISubWidgetText.h"

namespace MyGUI
{

	LayerNode::LayerNode(ILayer* _layer, ILayerNode* _parent) :
		mParent(_parent),
		mLayer(_layer)
	{
	}

	LayerNode::~LayerNode()
	{
		for (auto& firstRenderItem : mFirstRenderItems)
			delete firstRenderItem;
		mFirstRenderItems.clear();

		for (auto& secondRenderItem : mSecondRenderItems)
			delete secondRenderItem;
		mSecondRenderItems.clear();

		for (auto& childItem : mChildItems)
			delete childItem;
		mChildItems.clear();
	}

	ILayerNode* LayerNode::createChildItemNode()
	{
		LayerNode* layer = new LayerNode(mLayer, this);
		mChildItems.push_back(layer);

		mOutOfDate = true;

		return layer;
	}

	void LayerNode::destroyChildItemNode(ILayerNode* _node)
	{
		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
		{
			if ((*iter) == _node)
			{
				delete _node;
				mChildItems.erase(iter);

				mOutOfDate = true;

				return;
			}
		}
		MYGUI_EXCEPT("item node not found");
	}

	void LayerNode::upChildItemNode(ILayerNode* _item)
	{
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

	void LayerNode::renderToTarget(IRenderTarget* _target, bool _update)
	{
		mDepth = _target->getInfo().maximumDepth;

		if (mOutOfDateCompression)
		{
			updateCompression();
			mOutOfDateCompression = false;
		}

		// сначала отрисовываем свое
		for (auto& firstRenderItem : mFirstRenderItems)
			firstRenderItem->renderToTarget(_target, _update);

		for (auto& secondRenderItem : mSecondRenderItems)
			secondRenderItem->renderToTarget(_target, _update);

		// теперь отрисовываем дочерние узлы
		for (auto& childItem : mChildItems)
			childItem->renderToTarget(_target, _update);

		mOutOfDate = false;
	}

	void LayerNode::resizeView(const IntSize& _viewSize)
	{
		IntSize oldSize = mLayer->getSize();

		for (const auto& item : mLayerItems)
			item->resizeLayerItemView(oldSize, _viewSize);
	}

	ILayerItem* LayerNode::getLayerItemByPoint(int _left, int _top) const
	{
		// сначала пикаем детей
		for (const auto& childItem : mChildItems)
		{
			ILayerItem* item = childItem->getLayerItemByPoint(_left, _top);
			if (nullptr != item)
				return item;
		}

		for (const auto& layerItem : mLayerItems)
		{
			ILayerItem* item = layerItem->getLayerItemByPoint(_left, _top);
			if (nullptr != item)
				return item;
		}

		return nullptr;
	}

	RenderItem* LayerNode::addToRenderItem(ITexture* _texture, bool _firstQueue, bool _manualRender)
	{
		RenderItem* item = nullptr;
		if (_firstQueue)
			item = addToRenderItemFirstQueue(_texture, _manualRender);
		else
			item = addToRenderItemSecondQueue(_texture, _manualRender);

		mOutOfDate = false;
		return item;
	}

	RenderItem* LayerNode::addToRenderItemFirstQueue(ITexture* _texture, bool _manualRender)
	{
		if (mFirstRenderItems.empty() || _manualRender)
		{
			RenderItem* item = new RenderItem();
			item->setTexture(_texture);
			item->setManualRender(_manualRender);
			mLastNotEmptyItem = mFirstRenderItems.size();
			mFirstRenderItems.push_back(item);

			return item;
		}

		mOutOfDateCompression = true;
		mOutOfDate = true;

		// first queue keep order

		// use either last non-empty buffer if it have same texture
		// or empty buffer (if found in the end)
		if (mLastNotEmptyItem < mFirstRenderItems.size())
		{
			RenderItem* item = mFirstRenderItems[mLastNotEmptyItem];
			if (!item->getManualRender() && item->getTexture() == _texture)
			{
				return item;
			}
		}

		if (mLastNotEmptyItem + 1 < mFirstRenderItems.size())
		{
			++mLastNotEmptyItem;
			mFirstRenderItems[mLastNotEmptyItem]->setTexture(_texture);
			return mFirstRenderItems[mLastNotEmptyItem];
		}

		// not found, create new
		RenderItem* item = new RenderItem();
		item->setTexture(_texture);
		item->setManualRender(_manualRender);
		mLastNotEmptyItem = mFirstRenderItems.size();
		mFirstRenderItems.push_back(item);

		return item;
	}

	RenderItem* LayerNode::addToRenderItemSecondQueue(ITexture* _texture, bool _manualRender)
	{
		// order is not important in second queue
		// use first buffer with same texture or empty buffer
		for (auto& secondRenderItem : mSecondRenderItems)
		{
			if (secondRenderItem->getTexture() == _texture)
			{
				return secondRenderItem;
			}
			if (secondRenderItem->getNeedVertexCount() == 0)
			{
				secondRenderItem->setTexture(_texture);

				return secondRenderItem;
			}
		}

		// not found, create new
		RenderItem* item = new RenderItem();
		item->setTexture(_texture);
		item->setManualRender(_manualRender);
		mSecondRenderItems.push_back(item);

		return item;
	}

	void LayerNode::attachLayerItem(ILayerItem* _item)
	{
		mLayerItems.push_back(_item);
		_item->attachItemToNode(mLayer, this);

		mOutOfDate = true;
	}

	void LayerNode::detachLayerItem(ILayerItem* _item)
	{
		for (VectorLayerItem::iterator iter = mLayerItems.begin(); iter != mLayerItems.end(); ++iter)
		{
			if ((*iter) == _item)
			{
				mLayerItems.erase(iter);

				mOutOfDate = true;

				return;
			}
		}
		MYGUI_EXCEPT("layer item not found");
	}

	void LayerNode::outOfDate(RenderItem* _item)
	{
		mOutOfDate = true;
		if (_item)
			_item->outOfDate();
	}

	EnumeratorILayerNode LayerNode::getEnumerator() const
	{
		return EnumeratorILayerNode(mChildItems);
	}

	size_t LayerNode::getLayerNodeCount() const
	{
		return mChildItems.size();
	}

	ILayerNode* LayerNode::getLayerNodeAt(size_t _index) const
	{
		MYGUI_ASSERT_RANGE(_index, mChildItems.size(), "LayerNode::getLayerNodeAt");

		return mChildItems[_index];
	}

	void LayerNode::updateCompression()
	{
		if (mFirstRenderItems.size() <= 1)
			return;

		bool need_compression = false;
		for (const auto& firstRenderItem : mFirstRenderItems)
		{
			if (firstRenderItem->getNeedCompression())
			{
				firstRenderItem->setNeedCompression(false);
				need_compression = true;
				break;
			}
		}

		if (!need_compression)
		{
			mLastNotEmptyItem = mFirstRenderItems.size() - 1;
		}
		else
		{
			// pushing all empty buffers to the end of buffers list
			VectorRenderItem nonEmptyItems;
			nonEmptyItems.reserve(mFirstRenderItems.size());
			VectorRenderItem emptyItems;

			for (const auto& item : mFirstRenderItems)
			{
				if (item->getNeedVertexCount() == 0 && !item->getManualRender())
					emptyItems.push_back(item);
				else
					nonEmptyItems.push_back(item);
			}
			mLastNotEmptyItem = nonEmptyItems.size() - 1;
			nonEmptyItems.insert(nonEmptyItems.end(), emptyItems.begin(), emptyItems.end());
			std::swap(mFirstRenderItems, nonEmptyItems);
		}
	}

	ILayer* LayerNode::getLayer() const
	{
		return mLayer;
	}

	ILayerNode* LayerNode::getParent() const
	{
		return mParent;
	}

	bool LayerNode::isOutOfDate() const
	{
		for (const auto& firstRenderItem : mFirstRenderItems)
		{
			if (firstRenderItem->isOutOfDate())
				return true;
		}

		for (const auto& secondRenderItem : mSecondRenderItems)
		{
			if (secondRenderItem->isOutOfDate())
				return true;
		}

		for (const auto& childItem : mChildItems)
		{
			if (static_cast<const LayerNode*>(childItem)->isOutOfDate())
				return true;
		}

		return mOutOfDate;
	}

	float LayerNode::getNodeDepth() const
	{
		return mDepth;
	}

} // namespace MyGUI
