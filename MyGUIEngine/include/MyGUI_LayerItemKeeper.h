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
#ifndef __MYGUI_LAYER_ITEM_KEEPER_H__
#define __MYGUI_LAYER_ITEM_KEEPER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	class LayerItem;
	class IRenderItem;
	class LayerItemKeeper;
	class LayerKeeper;
	typedef std::vector<IRenderItem*> VectorRenderItem;
	typedef std::vector<LayerItem*> VectorLayerItem;
	typedef std::vector<LayerItemKeeper*> VectorLayerItemKeeper;

	class MYGUI_EXPORT LayerItemKeeper
	{
	public:
		explicit LayerItemKeeper(LayerKeeper * _layer, LayerItemKeeper * _parent = 0);
		~LayerItemKeeper();

		void _addUsing() { mCountUsing++; }
		void _removeUsing() { mCountUsing--; }
		size_t _countUsing() { return mCountUsing; }

		// возвращает колличество узлов
		size_t getItemCount();

		void _render(bool _update);

		IRenderItem* addToRenderItem(const std::string& _texture, bool _first);

		void addRenderItem(IRenderItem* _item);
		void removeRenderItem(IRenderItem* _item);

		LayerItem * _findLayerItem(int _left, int _top);

		void _addLayerItem(LayerItem * _root);

		void _removeLayerItem(LayerItem * _root);

		// обновляет очередь буферов
		void _update();

		LayerItemKeeper * getParent() { return mParent; }

		LayerItemKeeper * createItem();

		// удаляем дочку
		void destroyItem(LayerItemKeeper * _item);

		// удаляем себя
		void destroyItem();

		// поднимает свою дочку
		LayerItemKeeper * upItem(LayerItemKeeper * _item);

		// поднимает себя у родителей
		void upItem();

		bool existItem(LayerItemKeeper * _item);

	private:
		size_t mCountUsing;

		// список двух очередей отрисовки, для сабскинов и текста
		VectorRenderItem mFirstRenderItems;
		VectorRenderItem mSecondRenderItems;
		VectorRenderItem mOtherRenderItems;

		// список всех рутовых виджетов
		// у перекрывающегося слоя здесь только один
		VectorLayerItem mLayerItems;

		// список такиж как мы, для построения дерева
		VectorLayerItemKeeper mChildItems;

		LayerItemKeeper * mParent;
		LayerKeeper * mLayer;
	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_ITEM_KEEPER_H__
