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
