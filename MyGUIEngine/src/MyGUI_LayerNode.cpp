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
		mOutOfDate(false)
	{
	}

	LayerNode::~LayerNode()
	{
		for (VectorRenderItem::iterator iter = mFirstRenderItems.begin(); iter != mFirstRenderItems.end(); ++iter)
		{
			delete (*iter);
		}
		mFirstRenderItems.clear();

		for (VectorRenderItem::iterator iter = mSecondRenderItems.begin(); iter != mSecondRenderItems.end(); ++iter)
		{
			delete (*iter);
		}
		mSecondRenderItems.clear();

		// удаляем дочерние узлы
		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
		{
			delete (*iter);
		}
		mChildItems.clear();
	}

	ILayerNode* LayerNode::createChildItemNode()
	{
		LayerNode* layer = new LayerNode(mLayer, this);
		mChildItems.push_back(layer);
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
				return;
			}
		}
		MYGUI_EXCEPT("item node not found");
	}

	void LayerNode::renderToTarget(IRenderTarget* _target, bool _update)
	{
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
		{
			(*iter)->renderToTarget(_target, _update);
		}
		for (VectorRenderItem::iterator iter = mSecondRenderItems.begin(); iter != mSecondRenderItems.end(); ++iter)
		{
			(*iter)->renderToTarget(_target, _update);
		}

		// теперь отрисовываем дочерние узлы
		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
		{
			(*iter)->renderToTarget(_target, _update);
		}

		mOutOfDate = false;
	}

	ILayerItem* LayerNode::getLayerItemByPoint(int _left, int _top) const
	{
		// сначала пикаем детей
		for (VectorILayerNode::const_iterator iter = mChildItems.begin(); iter != mChildItems.end(); ++iter)
		{
			ILayerItem* item = (*iter)->getLayerItemByPoint(_left, _top);
			if (nullptr != item) return item;
		}

		for (VectorLayerItem::const_iterator iter = mLayerItems.begin(); iter != mLayerItems.end(); ++iter)
		{
			ILayerItem* item = (*iter)->getLayerItemByPoint(_left, _top);
			if (nullptr != item) return item;
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
						else if ((*next)->getTexture() == _texture)
							iter = next;
					}

					break;
				}

				(*iter)->setTexture(_texture);
				return (*iter);
			}
			// последний буфер с нужной текстурой
			else if ((*iter)->getTexture() == _texture)
			{
				return *iter;
			}

			// создаем новый буфер
			RenderItem* item = new RenderItem();
			item->setTexture(_texture);
			item->setManualRender(_manualRender);
			mFirstRenderItems.push_back(item);

			return item;
		}

		// для второй очереди порядок неважен
		for (VectorRenderItem::iterator iter = mSecondRenderItems.begin(); iter != mSecondRenderItems.end(); ++iter)
		{
			// либо такая же текстура, либо пустой буфер
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
		// не найденно создадим новый
		RenderItem* item = new RenderItem();
		item->setTexture(_texture);
		item->setManualRender(_manualRender);

		mSecondRenderItems.push_back(item);
		return mSecondRenderItems.back();
	}

	void LayerNode::attachLayerItem(ILayerItem* _item)
	{
		mLayerItems.push_back(_item);
		_item->attachItemToNode(mLayer, this);
	}

	void LayerNode::detachLayerItem(ILayerItem* _item)
	{
		for (VectorLayerItem::iterator iter = mLayerItems.begin(); iter != mLayerItems.end(); ++iter)
		{
			if ((*iter) == _item)
			{
				(*iter) = mLayerItems.back();
				mLayerItems.pop_back();
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
				if ((*iter1)->getNeedVertexCount() == 0)
				{
					RenderItem* tmp = (*iter1);
					(*iter1) = (*iter2);
					(*iter2) = tmp;
				}
				iter1 = iter2;
				++iter2;
			}
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
		return mOutOfDate;
	}

} // namespace MyGUI
