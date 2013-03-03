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

		// проверяем на сжатие пустот
		bool need_compression = false;
		for (VectorRenderItem::iterator iter = mFirstRenderItems.begin(); iter != mFirstRenderItems.end(); ++iter)
		{
			if ((*iter)->getCompression())
			{
				need_compression = true;
				break;
			}
		}

		if (need_compression)
			updateCompression();

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
		// для первичной очереди нужен порядок
		if (_firstQueue)
		{
			if (mFirstRenderItems.empty() || _manualRender)
			{
				// создаем новый буфер
				RenderItem* item = new RenderItem();
				item->setTexture(_texture);
				item->setManualRender(_manualRender);
				mFirstRenderItems.push_back(item);

				mOutOfDate = false;

				return item;
			}

			// если в конце пустой буфер, то нуна найти последний пустой с краю
			// либо с нужной текстурой за пустым
			VectorRenderItem::reverse_iterator iter = mFirstRenderItems.rbegin();
			if ((*iter)->getNeedVertexCount() == 0)
			{
				while (true)
				{
					VectorRenderItem::reverse_iterator next = iter + 1;
					if (next != mFirstRenderItems.rend())
					{
						if ((*next)->getNeedVertexCount() == 0)
						{
							iter = next;
							continue;
						}
						else if (!(*next)->getManualRender() && (*next)->getTexture() == _texture)
						{
							iter = next;
						}
					}

					break;
				}

				(*iter)->setTexture(_texture);

				mOutOfDate = false;

				return (*iter);
			}
			// последний буфер с нужной текстурой
			else if (!(*iter)->getManualRender() && (*iter)->getTexture() == _texture)
			{
				mOutOfDate = false;

				return *iter;
			}

			// создаем новый буфер
			RenderItem* item = new RenderItem();
			item->setTexture(_texture);
			item->setManualRender(_manualRender);
			mFirstRenderItems.push_back(item);

			mOutOfDate = false;

			return item;
		}

		// для второй очереди порядок неважен
		for (VectorRenderItem::iterator iter = mSecondRenderItems.begin(); iter != mSecondRenderItems.end(); ++iter)
		{
			// либо такая же текстура, либо пустой буфер
			if ((*iter)->getTexture() == _texture)
			{
				mOutOfDate = false;

				return (*iter);
			}
			else if ((*iter)->getNeedVertexCount() == 0)
			{
				(*iter)->setTexture(_texture);

				mOutOfDate = false;

				return (*iter);
			}
		}
		// не найденно создадим новый
		RenderItem* item = new RenderItem();
		item->setTexture(_texture);
		item->setManualRender(_manualRender);

		mSecondRenderItems.push_back(item);

		mOutOfDate = false;

		return mSecondRenderItems.back();
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
		// буферы освобождаются по одному всегда
		if (mFirstRenderItems.size() > 1)
		{
			// пытаемся поднять пустой буфер выше полных
			VectorRenderItem::iterator iter1 = mFirstRenderItems.begin();
			VectorRenderItem::iterator iter2 = iter1 + 1;
			while (iter2 != mFirstRenderItems.end())
			{
				if ((*iter1)->getNeedVertexCount() == 0 && !(*iter1)->getManualRender() && !(*iter2)->getManualRender())
				{
					RenderItem* tmp = (*iter1);
					(*iter1) = (*iter2);
					(*iter2) = tmp;
				}
				iter1 = iter2;
				++iter2;
			}
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
