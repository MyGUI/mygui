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

	class LayerItem;
	class LayerItemKeeper;
	typedef std::vector<LayerItemKeeper*> VectorLayerItemKeeper;

	class _MyGUIExport LayerKeeper
	{
	public:
		LayerKeeper(const std::string& _name, bool _overlapped, bool _peek);
		~LayerKeeper();

		inline const std::string& getName() { return mName; }

		LayerItemKeeper * getItem();
		void leaveItem(LayerItemKeeper * _item);
		void upItem(LayerItemKeeper * _item);

		void _render(bool _update);
		//void _resize(const FloatSize& _size);

		LayerItem * _findLayerItem(int _left, int _top, LayerItem* &_root);

	private:
		std::string mName;
		bool mIsOverlapped;
		bool mIsPeek;

		VectorLayerItemKeeper mLayerItemKeepers;

	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_KEEPER_H__
