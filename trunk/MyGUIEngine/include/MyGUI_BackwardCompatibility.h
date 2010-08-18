/*!
	@file
	@author		Albert Semenov
	@date		08/2010
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
#ifndef __MYGUI_BACKWARD_COMPATIBILITY_H__
#define __MYGUI_BACKWARD_COMPATIBILITY_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{
	class ButtonObsolete
	{
	public:
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : void Button::setStateSelected(bool _value)")
		void setButtonPressed(bool _value);
		MYGUI_OBSOLETE("use : bool Button::getStateSelected()")
		bool getButtonPressed();
		MYGUI_OBSOLETE("use : void Button::setStateSelected(bool _value)")
		void setStateCheck(bool _value);
		MYGUI_OBSOLETE("use : bool Button::getStateSelected()")
		bool getStateCheck();

#endif // MYGUI_DONT_USE_OBSOLETE
	};

	class ComboBoxObsolete
	{
	public:
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : size_t ComboBox::getIndexSelected()")
		size_t getItemIndexSelected();
		MYGUI_OBSOLETE("use : void ComboBox::setIndexSelected(size_t _index)")
		void setItemSelectedAt(size_t _index);
		MYGUI_OBSOLETE("use : void ComboBox::clearIndexSelected()")
		void clearItemSelected();

		MYGUI_OBSOLETE("use : void ComboBox::insertItemAt(size_t _index, const UString& _name)")
		void insertItem(size_t _index, const UString& _name);
		MYGUI_OBSOLETE("use : void ComboBox::setItemNameAt(size_t _index, const UString& _name)")
		void setItem(size_t _index, const UString& _item);
		MYGUI_OBSOLETE("use : const UString& ComboBox::getItemNameAt(size_t _index)")
		const UString& getItem(size_t _index);
		MYGUI_OBSOLETE("use : void ComboBox::removeItemAt(size_t _index)")
		void deleteItem(size_t _index);
		MYGUI_OBSOLETE("use : void ComboBox::removeAllItems()")
		void deleteAllItems();
		MYGUI_OBSOLETE("use : size_t ComboBox::getIndexSelected()")
		size_t getItemSelect();
		MYGUI_OBSOLETE("use : void void ComboBox::clearIndexSelected()")
		void resetItemSelect();
		MYGUI_OBSOLETE("use : void ComboBox::setIndexSelected(size_t _index)")
		void setItemSelect(size_t _index);

		MYGUI_OBSOLETE("use : void ComboBox::setMaxListLength(int _value)")
		void setMaxListHeight(int _value);
		MYGUI_OBSOLETE("use : int ComboBox::getMaxListLength()")
		int getMaxListHeight();

#endif // MYGUI_DONT_USE_OBSOLETE
	};

} // namespace MyGUI

#endif // __MYGUI_BACKWARD_COMPATIBILITY_H__
