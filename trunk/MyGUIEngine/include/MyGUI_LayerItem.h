/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_LAYER_ITEM_H__
#define __MYGUI_LAYER_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"

namespace MyGUI
{

	class LayerKeeper;
	class LayerItemKeeper;
	class LayerManager;

	class _MyGUIExport LayerItem
	{
		friend class LayerItemKeeper;
		friend class LayerManager;

	public:
		LayerItem() : mLayerKeeper(null), mLayerItemKeeper(null) { }
		virtual ~LayerItem() { }

	private:
		virtual LayerItem * _findLayerItem(int _left, int _top) = 0;

	public:

		LayerKeeper * getLayerKeeper() { return mLayerKeeper; }

		// наш текущий хранитель леера
		LayerItemKeeper * getLayerItemKeeper() { return mLayerItemKeeper; }
		void setLayerItemKeeper(LayerItemKeeper * _item) { mLayerItemKeeper = _item; }

		// физическое подсоединение и отсоединение
		virtual void _attachToLayerItemKeeper(LayerItemKeeper * _item, bool _deep = false) = 0;
		virtual void _detachFromLayerItemKeeper(bool _deep = false) = 0;

	private:
		// актуально для рутового виджета
		LayerKeeper * mLayerKeeper;
		// конкретный айтем находящийся в слое
		LayerItemKeeper * mLayerItemKeeper;

	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_ITEM_H__
