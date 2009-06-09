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
#include "MyGUI_LayerNode.h"
#include "MyGUI_ILayerItem.h"
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
		for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter)
		{
			delete (*iter);
		}
		mFirstRenderItems.clear();

		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter)
		{
			delete (*iter);
		}
		mSecondRenderItems.clear();

		// удаляем дочерние узлы
		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter!=mChildItems.end(); ++iter)
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
		for (VectorILayerNode::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
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

	void LayerNode::renderToTarget(IRenderTarget* _target, bool _update)
	{
		// сначала отрисовываем свое
		for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter)
		{
			(*iter)->renderToTarget(_target, _update);
		}
		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter)
		{
			(*iter)->renderToTarget(_target, _update);
		}

		// теперь отрисовываем дочерние узлы
		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			(*iter)->renderToTarget(_target, _update);
		}
	}

	ILayerItem* LayerNode::getLayerItemByPoint(int _left, int _top)
	{
		// сначала пикаем детей
		for (VectorILayerNode::iterator iter = mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			ILayerItem * item = (*iter)->getLayerItemByPoint(_left, _top);
			if (nullptr != item) return item;
		}

		for (VectorLayerItem::iterator iter=mLayerItems.begin(); iter!=mLayerItems.end(); ++iter)
		{
			ILayerItem * item = (*iter)->getLayerItemByPoint(_left, _top);
			if (nullptr != item) return item;
		}

		return nullptr;
	}

	RenderItem* LayerNode::addToRenderItem(const std::string& _texture, ISubWidget* _item)
	{
		bool first = _item->castType<ISubWidgetText>(false) == nullptr;
		// для первичной очереди нужен порядок
		if (first)
		{
			if (mFirstRenderItems.empty())
			{
				// создаем новый буфер
				RenderItem * item = new RenderItem(_texture);
				mFirstRenderItems.push_back(item);

				return item;
			}

			// если последний буфер пустой, то мона не создавать
			if (mFirstRenderItems.back()->getNeedVertexCount() == 0)
			{
				// пустых может быть сколько угодно, нужен самый первый из пустых
				for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter)
				{
					if ((*iter)->getNeedVertexCount() == 0)
					{
						// а теперь внимание, если перед пустым наш, то его и юзаем
						if (iter != mFirstRenderItems.begin())
						{
							VectorRenderItem::iterator prev = iter - 1;
							if ((*prev)->getTextureName() == _texture)
							{
								return (*prev);
							}
						}
						(*iter)->setTextureName(_texture);
						return (*iter);
					}
				}
			}

			// та же текстура
			if (mFirstRenderItems.back()->getTextureName() == _texture)
			{
				return mFirstRenderItems.back();
			}

			// создаем новый буфер
			RenderItem * item = new RenderItem(_texture);
			mFirstRenderItems.push_back(item);

			return item;
		}

		// для второй очереди порядок неважен
		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter)
		{
			// либо такая же текстура, либо пустой буфер
			if ((*iter)->getTextureName() == _texture)
			{
				return (*iter);
			}
			else if ((*iter)->getNeedVertexCount() == 0)
			{
				(*iter)->setTextureName(_texture);
				return (*iter);
			}

		}
		// не найденно создадим новый
		mSecondRenderItems.push_back(new RenderItem(_texture));
		return mSecondRenderItems.back();
	}

	void LayerNode::attachLayerItem(ILayerItem* _item)
	{
		mLayerItems.push_back(_item);
		_item->attachItemToNode(mLayer, this);
	}

	void LayerNode::detachLayerItem(ILayerItem* _item)
	{
		for (VectorLayerItem::iterator iter=mLayerItems.begin(); iter!=mLayerItems.end(); ++iter)
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
		_item->outOfDate();
	}

	EnumeratorILayerNode LayerNode::getEnumerator()
	{
		return EnumeratorILayerNode(mChildItems);
	}

} // namespace MyGUI
