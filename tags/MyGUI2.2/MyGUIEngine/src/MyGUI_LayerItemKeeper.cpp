/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*//*
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
#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerItem.h"
#include "MyGUI_LayerKeeper.h"

namespace MyGUI
{

	LayerItemKeeper::LayerItemKeeper(LayerKeeper * _layer, LayerItemKeeper * _parent) :
		mCountUsing(0),
		mParent(_parent),
		mLayer(_layer)
	{
	}

	LayerItemKeeper::~LayerItemKeeper()
	{
		for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter)
		{
			delete (*iter);
		}
		mFirstRenderItems.clear();

		for (VectorRenderItem::iterator iter=mOtherRenderItems.begin(); iter!=mOtherRenderItems.end(); ++iter)
		{
			delete (*iter);
		}
		mOtherRenderItems.clear();

		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter)
		{
			delete (*iter);
		}
		mSecondRenderItems.clear();

		// удаляем дочерние узлы
		for (VectorLayerItemKeeper::iterator iter = mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			delete (*iter);
		}
		mChildItems.clear();
	}

	LayerItemKeeper * LayerItemKeeper::createItem()
	{
		LayerItemKeeper * layer = new LayerItemKeeper(mLayer, this);
		mChildItems.push_back(layer);
		layer->_addUsing();
		return layer;
	}

	void LayerItemKeeper::destroyItem()
	{
		mLayer->destroyItem(this);
	}

	void LayerItemKeeper::destroyItem(LayerItemKeeper * _item)
	{
		for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			if ((*iter) == _item)
			{
				_item->_removeUsing();
				if (0 == _item->_countUsing())
				{
					delete _item;
					mChildItems.erase(iter);
				}
				return;
			}
		}
		MYGUI_EXCEPT("item keeper not found");
	}

	LayerItemKeeper * LayerItemKeeper::upItem(LayerItemKeeper * _item)
	{
		// поднимаем с себя до самого верхнего родителя
		if ((2 > mChildItems.size()) || (mChildItems.back() == _item))
		{
			return mParent ? mParent->upItem(this) : this;
		}
		for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter) {
			if ((*iter) == _item) {
				mChildItems.erase(iter);
				mChildItems.push_back(_item);
				return mParent ? mParent->upItem(this) : this;
			}
		}

		MYGUI_EXCEPT("item keeper not found");
	}

	void LayerItemKeeper::upItem()
	{
		mLayer->upItem(this);
	}

	void LayerItemKeeper::_render(bool _update)
	{

		// сначала отрисовываем свое
		for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter) {
			(*iter)->_render(_update);
		}
		for (VectorRenderItem::iterator iter=mOtherRenderItems.begin(); iter!=mOtherRenderItems.end(); ++iter) {
			(*iter)->_render(_update);
		}
		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter) {
			(*iter)->_render(_update);
		}

		// теперь отрисовываем дочерние узлы
		for (VectorLayerItemKeeper::iterator iter = mChildItems.begin(); iter!=mChildItems.end(); ++iter) {
			(*iter)->_render(_update);
		}

	}

	LayerItem * LayerItemKeeper::_findLayerItem(int _left, int _top)
	{
		// сначала пикаем детей
		for (VectorLayerItemKeeper::iterator iter = mChildItems.begin(); iter!=mChildItems.end(); ++iter) {
			LayerItem * item = (*iter)->_findLayerItem(_left, _top);
			if (nullptr != item) return item;
		}

		// а теперь себя
		for (VectorLayerItem::iterator iter=mLayerItems.begin(); iter!=mLayerItems.end(); ++iter) {
			LayerItem * item = (*iter)->_findLayerItem(_left, _top);
			if (nullptr != item) return item;
		}

		return nullptr;
	}

	void LayerItemKeeper::_update()
	{
		// буферы освобождаются по одному всегда
		if (mFirstRenderItems.size() > 1)
		{
			// пытаемся поднять пустой буфер выше полных
			VectorRenderItem::iterator iter1 = mFirstRenderItems.begin();
			VectorRenderItem::iterator iter2 = iter1 + 1;
			while (iter2 != mFirstRenderItems.end())
			{
				if ((*iter1)->empty())
				{
					IRenderItem * tmp = (*iter1);
					(*iter1) = (*iter2);
					(*iter2) = tmp;
				}
				iter1 = iter2;
				++iter2;
			}
		}
	}

	IRenderItem * LayerItemKeeper::addToRenderItem(const std::string& _texture, const std::string& _type)
	{
		// для первичной очереди нужен порядок
		if (_type == "First")
		{
			if (mFirstRenderItems.empty())
			{
				// создаем новый буфер
				RenderItem * item = new RenderItem(_texture, this);
				mFirstRenderItems.push_back(item);

				return item;
			}

			// если последний буфер пустой, то мона не создавать
			if (mFirstRenderItems.back()->empty())
			{
				// пустых может быть сколько угодно, нужен самый первый из пустых
				for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter)
				{
					if ((*iter)->empty())
					{
						static_cast<RenderItem*>(*iter)->setTextureName(_texture);
						return (*iter);
					}
				}
			}

			// та же текстура
			if (static_cast<RenderItem*>(mFirstRenderItems.back())->getTextureName() == _texture)
			{
				return mFirstRenderItems.back();
			}

			// создаем новый буфер
			RenderItem * item = new RenderItem(_texture, this);
			mFirstRenderItems.push_back(item);

			return item;
		}
		//else if (_type == "Second")
		//{
			// для второй очереди порядок неважен
			for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter)
			{
				// либо такая же текстура, либо пустой буфер
				if (static_cast<RenderItem*>(*iter)->getTextureName() == _texture)
				{
					return (*iter);
				}
				else if ((*iter)->empty())
				{
					static_cast<RenderItem*>(*iter)->setTextureName(_texture);
					return (*iter);
				}

			}
			// не найденно создадим новый
			mSecondRenderItems.push_back(new RenderItem(_texture, this));
			return mSecondRenderItems.back();
		//}

	}

	size_t LayerItemKeeper::getItemCount()
	{
		size_t count = 1;
		// сначала пикаем детей
		for (VectorLayerItemKeeper::iterator iter = mChildItems.begin(); iter!=mChildItems.end(); ++iter) {
			count += (*iter)->getItemCount();
		}
		return count;
	}

	bool LayerItemKeeper::existItem(LayerItemKeeper * _item)
	{
		for (VectorLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter) {
			if ((*iter) == _item || (*iter)->existItem(_item)) return true;
		}
		return false;
	}

	void LayerItemKeeper::_addLayerItem(LayerItem * _root)
	{
		mLayerItems.push_back(_root);
	}

	void LayerItemKeeper::_removeLayerItem(LayerItem * _root)
	{
		for (VectorLayerItem::iterator iter=mLayerItems.begin(); iter!=mLayerItems.end(); ++iter) {
			if ((*iter) == _root) {
				(*iter) = mLayerItems.back();
				mLayerItems.pop_back();
				return;
			}
		}
		MYGUI_EXCEPT("layer item not found");
	}

	void LayerItemKeeper::addRenderItem(IRenderItem* _item)
	{
		mOtherRenderItems.push_back(_item);
	}

	void LayerItemKeeper::removeRenderItem(IRenderItem* _item)
	{
		mOtherRenderItems.erase(std::find(mOtherRenderItems.begin(), mOtherRenderItems.end(), _item));
	}

} // namespace MyGUI
