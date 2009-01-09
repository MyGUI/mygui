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

	class MYGUI_EXPORT LayerKeeper
	{
	public:
		LayerKeeper(const std::string& _name, bool _overlapped, bool _pick);
		~LayerKeeper();

		const std::string& getName() { return mName; }
		size_t getItemCount();
		size_t getSubItemCount();

		LayerItemKeeper * createItem(LayerItemKeeper * _parent = 0);
		void destroyItem(LayerItemKeeper * _item);
		void upItem(LayerItemKeeper * _item);

		void _render(bool _update);

		LayerItem * _findLayerItem(int _left, int _top);

		bool existItem(LayerItemKeeper * _item);

	private:
		std::string mName;
		bool mIsOverlapped;
		bool mIsPeek;

		VectorLayerItemKeeper mChildItems;

	};

} // namespace MyGUI

#endif // __MYGUI_LAYER_KEEPER_H__
