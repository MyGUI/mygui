/*!
	@file
	@author		Albert Semenov
	@date		11/2010
*/
/*
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
#ifndef __MYGUI_I_ITEM_CONTAINER_H__
#define __MYGUI_I_ITEM_CONTAINER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_StaticText.h"

namespace MyGUI
{

	class MYGUI_EXPORT IItemContainer
	{
	public:
		IItemContainer() { }
		virtual ~IItemContainer() { }

		virtual size_t getItemCount() const { return 0; }

		virtual void addItem(const MyGUI::UString& _name) { }
		virtual void insertItemAt(size_t _index, const MyGUI::UString& _name) { }

		virtual void removeItemAt(size_t _index) { }
		virtual void removeAllItems() { }

		virtual void setItemNameAt(size_t _index, const UString& _name) { }
		virtual const UString& getItemNameAt(size_t _index) { return mEmptyName; }

	private:
		MyGUI::UString mEmptyName;
	};

} // namespace MyGUI

#endif // __MYGUI_I_ITEM_CONTAINER_H__
