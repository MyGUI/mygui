/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_LAYER_ITEM_KEEPER_H__
#define __MYGUI_LAYER_ITEM_KEEPER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	class LayerItem;
	class RenderItem;
	class LayerItemKeeper;
	class LayerKeeper;
	typedef std::vector<RenderItem*> VectorRenderItem;
	typedef std::vector<LayerItem*> VectorLayerItem;
	typedef std::vector<LayerItemKeeper*> VectorLayerItemKeeper;

	class _MyGUIExport LayerItemKeeper
	{
	public:
		explicit LayerItemKeeper(LayerKeeper * _layer, LayerItemKeeper * _parent = 0);
		~LayerItemKeeper();

		void _addUsing() { mCountUsing++; }
		void _removeUsing() { mCountUsing--; }
		size_t _countUsing() { return mCountUsing; }

		void _render(bool _update);

		RenderItem * addToRenderItem(const std::string& _texture, bool _first, bool _separate);

		LayerItem * _findLayerItem(int _left, int _top);

		void _addPeekItem(LayerItem * _root)
		{
			mPeekLayerItems.push_back(_root);
		}

		void _removePeekItem(LayerItem * _root)
		{
			for (VectorLayerItem::iterator iter=mPeekLayerItems.begin(); iter!=mPeekLayerItems.end(); ++iter) {
				if ((*iter) == _root) {
					(*iter) = mPeekLayerItems.back();
					mPeekLayerItems.pop_back();
					break;
				}
			}
		}

		// обновляет очередь буферов
		void _update();

		LayerItemKeeper * getParent() { return mParent; }

		LayerItemKeeper * createItem();

		void destroyItem(LayerItemKeeper * _item);

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

		// список всех рутовых виджетов
		// у перекрывающегося слоя здесь только один
		VectorLayerItem mPeekLayerItems;

		// список такиж как мы, для построения дерева
		VectorLayerItemKeeper mChildItems;

		LayerItemKeeper * mParent;
		LayerKeeper * mLayer;
	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_ITEM_KEEPER_H__
