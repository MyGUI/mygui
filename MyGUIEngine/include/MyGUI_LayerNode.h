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
#ifndef __MYGUI_LAYER_NODE_H__
#define __MYGUI_LAYER_NODE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_ILayer.h"
#include "MyGUI_ILayerNode.h"
#include "MyGUI_RenderItem.h"

namespace MyGUI
{

	class LayerItem;
	class LayerNode;
	typedef std::vector<RenderItem*> VectorRenderItem;
	typedef std::vector<ILayerItem*> VectorLayerItem;
	typedef std::vector<LayerNode*> VectorLayerItemKeeper;

	class MYGUI_EXPORT LayerNode : public ILayerNode
	{
	public:
		explicit LayerNode(ILayer* _layer, LayerNode * _parent = 0);
		virtual ~LayerNode();

		void _addUsing() { mCountUsing++; }
		void _removeUsing() { mCountUsing--; }
		size_t _countUsing() { return mCountUsing; }

		// возвращает колличество узлов
		size_t getItemCount();

		void _render(bool _update);

		ILayerItem * getLayerItemByPoint(int _left, int _top);

		virtual void attachLayerItem(ILayerItem* _root);
		virtual void detachLayerItem(ILayerItem* _root);

		// обновляет очередь буферов
		void _update();

		LayerNode * getParent() { return mParent; }

		virtual ILayerNode* createItemNode();

		// удаляем дочку
		void destroyItemNode(LayerNode* _item);

		// удаляем себя
		virtual void destroyItemNode();

		// поднимает свою дочку
		LayerNode * upItemNode(LayerNode* _item);

		// поднимает себя у родителей
		void upItemNode();

		bool existItemNode(LayerNode* _item);

		virtual ILayer* getLayer();

		virtual IRenderItem * addToRenderItem(const std::string& _texture, IDrawItem* _item);

	private:
		IRenderItem * addToRenderItem(const std::string& _texture, bool _first);

	private:
		size_t mCountUsing;

		// список двух очередей отрисовки, для сабскинов и текста
		VectorRenderItem mFirstRenderItems;
		VectorRenderItem mSecondRenderItems;

		// список всех рутовых виджетов
		// у перекрывающегося слоя здесь только один
		VectorLayerItem mLayerItems;
		//LayerItem* mLayerItem;

		// список такиж как мы, для построения дерева
		VectorLayerItemKeeper mChildItems;

		LayerNode * mParent;
		ILayer* mLayer;
	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_NODE_H__
