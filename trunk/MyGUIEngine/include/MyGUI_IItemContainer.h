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
#include "MyGUI_Constants.h"

namespace MyGUI
{

	class IItem;

	class MYGUI_EXPORT IItemContainer
	{
	public:
		IItemContainer() { }
		virtual ~IItemContainer() { }

		virtual size_t _getItemCount()
		{
			return 0;
		}

		virtual void _addItem(const MyGUI::UString& _name) { }

		virtual void _removeItemAt(size_t _index) { }

		virtual Widget* _getItemAt(size_t _index)
		{
			return nullptr;
		}

		virtual void _setItemNameAt(size_t _index, const UString& _name) { }
		virtual const UString& _getItemNameAt(size_t _index)
		{
			return Constants::getEmptyUString();
		}

		virtual void _setItemSelected(IItem* _item) { }
	};

} // namespace MyGUI

#endif // __MYGUI_I_ITEM_CONTAINER_H__
