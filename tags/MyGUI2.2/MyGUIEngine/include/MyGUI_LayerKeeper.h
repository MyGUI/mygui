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
