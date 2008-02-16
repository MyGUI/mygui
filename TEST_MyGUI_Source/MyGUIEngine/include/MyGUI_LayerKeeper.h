/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_LAYER_KEEPER_H__
#define __MYGUI_LAYER_KEEPER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	class LayerItemKeeper;
	typedef std::vector<LayerItemKeeper*> VectorLayerItemKeeper;

	class _MyGUIExport LayerKeeper
	{
	public:
		LayerKeeper(const std::string& _name, bool _overlapped);

		inline const std::string& getName() { return mName; }

		LayerItemKeeper * getItem();
		void leaveItem(LayerItemKeeper * _item);
		void upItem(LayerItemKeeper * _item);

		void _render();
		void _resize(const FloatSize& _size);

	private:
		std::string mName;
		bool mIsOverlapped;

		VectorLayerItemKeeper mLayerItemKeepers;

	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_KEEPER_H__
