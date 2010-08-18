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
#include "MyGUI_Precompiled.h"
#include "MyGUI_BackwardCompatibility.h"
#include "MyGUI_Button.h"
#include "MyGUI_ComboBox.h"

namespace MyGUI
{

#ifndef MYGUI_DONT_USE_OBSOLETE

	void ButtonObsolete::setButtonPressed(bool _value) { static_cast<Button*>(this)->setStateSelected(_value); }
	bool ButtonObsolete::getButtonPressed() { return static_cast<Button*>(this)->getStateSelected(); }
	void ButtonObsolete::setStateCheck(bool _value) { static_cast<Button*>(this)->setStateSelected(_value); }
	bool ButtonObsolete::getStateCheck() { return static_cast<Button*>(this)->getStateSelected(); }

#endif // MYGUI_DONT_USE_OBSOLETE

#ifndef MYGUI_DONT_USE_OBSOLETE

	size_t ComboBoxObsolete::getItemIndexSelected() { return static_cast<ComboBox*>(this)->getIndexSelected(); }
	void ComboBoxObsolete::setItemSelectedAt(size_t _index) { static_cast<ComboBox*>(this)->setIndexSelected(_index); }
	void ComboBoxObsolete::clearItemSelected() { static_cast<ComboBox*>(this)->clearIndexSelected(); }

	void ComboBoxObsolete::insertItem(size_t _index, const UString& _name) { static_cast<ComboBox*>(this)->insertItemAt(_index, _name); }
	void ComboBoxObsolete::setItem(size_t _index, const UString& _item) { static_cast<ComboBox*>(this)->setItemNameAt(_index, _item); }
	const UString& ComboBoxObsolete::getItem(size_t _index) { return static_cast<ComboBox*>(this)->getItemNameAt(_index); }
	void ComboBoxObsolete::deleteItem(size_t _index) { static_cast<ComboBox*>(this)->removeItemAt(_index); }
	void ComboBoxObsolete::deleteAllItems() { static_cast<ComboBox*>(this)->removeAllItems(); }
	size_t ComboBoxObsolete::getItemSelect() { return static_cast<ComboBox*>(this)->getIndexSelected(); }
	void ComboBoxObsolete::resetItemSelect() { static_cast<ComboBox*>(this)->clearIndexSelected(); }
	void ComboBoxObsolete::setItemSelect(size_t _index) { static_cast<ComboBox*>(this)->setIndexSelected(_index); }

	void ComboBoxObsolete::setMaxListHeight(int _value) { static_cast<ComboBox*>(this)->setMaxListLength(_value); }
	int ComboBoxObsolete::getMaxListHeight() { return static_cast<ComboBox*>(this)->getMaxListLength(); }

#endif // MYGUI_DONT_USE_OBSOLETE

} // namespace MyGUI
