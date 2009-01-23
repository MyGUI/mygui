/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_LAYER_ITEM_H__
#define __MYGUI_LAYER_ITEM_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	class LayerKeeper;
	class LayerItemKeeper;
	class LayerManager;

	class MYGUI_EXPORT LayerItem
	{
		friend class LayerItemKeeper;
		friend class LayerManager;

	public:
		LayerItem() : mLayerKeeper(nullptr), mLayerItemKeeper(nullptr) { }
		virtual ~LayerItem() { }

	private:
		virtual LayerItem * _findLayerItem(int _left, int _top) = 0;

	public:

		LayerKeeper * getLayerKeeper() { return mLayerKeeper; }

		/** Get LayerItem keeper for this item */
		LayerItemKeeper * getLayerItemKeeper()
		{
#if MYGUI_DEBUG_MODE == 1
			if (mLayerItemKeeper) {
				MYGUI_ASSERT(LayerManager::getInstance().isExistItem(mLayerItemKeeper), "layer item is not exist");
			}
#endif
			return mLayerItemKeeper;
		}
		/** Set LayerItem keeper for this item */
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
