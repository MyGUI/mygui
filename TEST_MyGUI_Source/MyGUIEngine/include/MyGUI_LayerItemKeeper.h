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
	typedef std::vector<RenderItem*> VectorRenderItem;
	typedef std::vector<LayerItem*> VectorLayerItem;

	class _MyGUIExport LayerItemKeeper
	{
	public:
		LayerItemKeeper();
		~LayerItemKeeper();

		inline void _addUsing() { mCountUsing++; }
		inline void _removeUsing() { mCountUsing--; }
		inline size_t _countUsing() { return mCountUsing; }

		void _render(bool _update);
		//void _resize(const FloatSize& _size);

		RenderItem * addToRenderItem(const std::string& _texture, bool _first, bool _separate);

		LayerItem * _findLayerItem(int _left, int _top, LayerItem* &_root);

		inline void _addPeekItem(LayerItem * _root)
		{
			mPeekLayerItems.push_back(_root);
		}

		inline void _removePeekItem(LayerItem * _root)
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

	private:
		size_t mCountUsing;

		// список двух очередей отрисовки, для сабскинов и текста
		VectorRenderItem mFirstRenderItems;
		VectorRenderItem mSecondRenderItems;

		// список всех рутовых виджетов
		// у перекрывающегося слоя здесь только один
		VectorLayerItem mPeekLayerItems;

	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_ITEM_KEEPER_H__
