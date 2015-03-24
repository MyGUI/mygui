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
		mLayer(_layer),
		mOutOfDate(false),
		mDepth(0.0f)
	{
	}

	LayerNode::~LayerNode()
	{
		for (VectorRenderItem::iterator iter = mFirstRenderItems.begin(); iter != mFirstRenderItems.end(); ++iter)
			delete (*iter);
		mFirstRenderItems.clear();

		for (VectorRenderItem::iterator iter = mSecondRenderItems.begin(); iter != mSecondRenderItems.end(); ++iter)
			delete (*iter);
		mSecondRenderItems.clear();

		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
			delete (*iter);
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

		// сначала отрисовываем свое
		for (VectorRenderItem::iterator iter = mFirstRenderItems.begin(); iter != mFirstRenderItems.end(); ++iter)
			(*iter)->renderToTarget(_target, _update);

		for (VectorRenderItem::iterator iter = mSecondRenderItems.begin(); iter != mSecondRenderItems.end(); ++iter)
			(*iter)->renderToTarget(_target, _update);

		// теперь отрисовываем дочерние узлы
		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
			(*iter)->renderToTarget(_target, _update);

		mOutOfDate = false;
	}

	void LayerNode::resizeView(const IntSize& _viewSize)
	{
		IntSize oldSize = mLayer->getSize();

		for (VectorLayerItem::const_iterator iter = mLayerItems.begin(); iter != mLayerItems.end(); ++iter)
			(*iter)->resizeLayerItemView(oldSize, _viewSize);
	}

	ILayerItem* LayerNode::getLayerItemByPoint(int _left, int _top) const
	{
		// сначала пикаем детей
		for (VectorILayerNode::const_iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
		{
			ILayerItem* item = (*iter)->getLayerItemByPoint(_left, _top);
			if (nullptr != item)
				return item;
		}

		for (VectorLayerItem::const_iterator iter = mLayerItems.begin(); iter != mLayerItems.end(); ++iter)
		{
			ILayerItem* item = (*iter)->getLayerItemByPoint(_left, _top);
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
			mFirstRenderItems.push_back(item);

			return item;
		}

		updateCompression();

		// first queue keep order

		// use either first empty buffer
		// or last non-empty buffer if it have same texture
		VectorRenderItem::reverse_iterator iter = mFirstRenderItems.rbegin();
		bool itemFound = false;
		while (iter != mFirstRenderItems.rend())
		{
			if ((*iter)->getNeedVertexCount() == 0)
			{
				iter++;
				itemFound = true;
				continue;
			}
			else if (!(*iter)->getManualRender() && (*iter)->getTexture() == _texture)
			{
				iter++;
				itemFound = true;
				break;
			}

			break;
		}

		if (itemFound)
		{
			iter--;
			(*iter)->setTexture(_texture);

			return (*iter);
		}

		// not found, create new
		RenderItem* item = new RenderItem();
		item->setTexture(_texture);
		item->setManualRender(_manualRender);
		mFirstRenderItems.push_back(item);

		return item;
	}

	RenderItem* LayerNode::addToRenderItemSecondQueue(ITexture* _texture, bool _manualRender)
	{
		// order is not important in second queue
		// use first buffer with same texture or empty buffer
		for (VectorRenderItem::iterator iter = mSecondRenderItems.begin(); iter != mSecondRenderItems.end(); ++iter)
		{
			if ((*iter)->getTexture() == _texture)
			{
				return (*iter);
			}
			else if ((*iter)->getNeedVertexCount() == 0)
			{
				(*iter)->setTexture(_texture);

				return (*iter);
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
		for (VectorRenderItem::iterator iter = mFirstRenderItems.begin(); iter != mFirstRenderItems.end(); ++iter)
		{
			if ((*iter)->getNeedCompression())
			{
				need_compression = true;
				break;
			}
		}

		// pushing all empty buffers to the end of buffers list
		if (need_compression)
		{
			VectorRenderItem nonEmptyItems;
			VectorRenderItem emptyItems;

			for (VectorRenderItem::iterator iter = mFirstRenderItems.begin(); iter != mFirstRenderItems.end(); ++iter)
			{
				(*iter)->setNeedCompression(false);

				if ((*iter)->getNeedVertexCount() == 0 && !(*iter)->getManualRender())
					emptyItems.push_back(*iter);
				else
					nonEmptyItems.push_back(*iter);
			}
			nonEmptyItems.insert(nonEmptyItems.end(), emptyItems.begin(), emptyItems.end());
			std::swap(mFirstRenderItems, nonEmptyItems);
		}

		mOutOfDate = true;
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
		for (VectorRenderItem::const_iterator item = mFirstRenderItems.begin(); item != mFirstRenderItems.end(); ++item)
		{
			if ((*item)->isOutOfDate())
				return true;
		}

		for (VectorRenderItem::const_iterator item = mSecondRenderItems.begin(); item != mSecondRenderItems.end(); ++item)
		{
			if ((*item)->isOutOfDate())
				return true;
		}

		for (VectorILayerNode::const_iterator item = mChildItems.begin(); item != mChildItems.end(); ++item)
		{
			if (static_cast<const LayerNode*>(*item)->isOutOfDate())
				return true;
		}

		return mOutOfDate;
	}

	float LayerNode::getNodeDepth()
	{
		return mDepth;
	}

} // namespace MyGUI
