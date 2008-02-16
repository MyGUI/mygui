/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_LAYER_ITEM_H__
#define __MYGUI_LAYER_ITEM_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class LayerKeeper;

	class _MyGUIExport LayerItem
	{

	public:
		virtual LayerItem * _findLayerItem(int _left, int _top) = 0;

		virtual void _attachToLayerKeeper(LayerKeeper * _item) = 0;
		virtual void _detachFromLayerKeeper() = 0;

	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_ITEM_H__
