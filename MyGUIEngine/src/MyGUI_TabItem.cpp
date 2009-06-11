/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
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
#include "MyGUI_TabItem.h"

namespace MyGUI
{

	TabItem::TabItem(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name) :
		Base(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name)
	{
		mOwner = getParent()->castType<Tab>();
	}

	TabItem::TabItem() :
		Base()
	{
	}

	void TabItem::_initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
	{
		Base::_initialise(_style, _coord, _align, _info, _parent, _croppedParent, _creator, _name);

		mOwner = getParent()->castType<Tab>();
	}

	TabItem::~TabItem()
	{
		mOwner->_notifyDeleteItem(this);
	}

	void TabItem::setSelected(bool _value)
	{
		if (_value) setItemSelected();
	}

	void TabItem::setProperty(const std::string& _key, const std::string& _value)
	{
		if (_key == "TabItem_ButtonWidth") setButtonWidth(utility::parseValue<int>(_value));
		else if (_key == "TabItem_Select") setSelected(utility::parseValue<bool>(_value));

#ifndef MYGUI_DONT_USE_OBSOLETE
		else if (_key == "Sheet_ButtonWidth")
		{
			MYGUI_LOG(Warning, "Sheet_ButtonWidth is obsolete, use TabItem_ButtonWidth");
			setButtonWidth(utility::parseValue<int>(_value));
		}
		else if (_key == "Sheet_Select")
		{
			MYGUI_LOG(Warning, "Sheet_Select is obsolete, use TabItem_Select");
			setSelected(utility::parseValue<bool>(_value));
		}
#endif // MYGUI_DONT_USE_OBSOLETE

		else Base::setProperty(_key, _value);
	}

} // namespace MyGUI
