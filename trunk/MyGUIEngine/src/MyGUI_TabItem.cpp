/*!
	@file
	@author		Albert Semenov
	@date		01/2008
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

	TabItem::TabItem()// :
		//mOwner(nullptr)
	{
	}

	void TabItem::initialiseWidgetSkin(ResourceSkin* _info)
	{
		Base::initialiseWidgetSkin(_info);

		// FIXME проверить смену скина ибо должно один раз вызываться
		//mOwner = getParent()->castType<Tab>();
	}

	void TabItem::shutdownWidgetSkin()
	{
		Tab* owner = getParent() != nullptr ? getParent()->castType<Tab>(false) : nullptr;
		if (owner != nullptr)
			owner->_notifyDeleteItem(this);

		Base::shutdownWidgetSkin();
	}

	/*void TabItem::setSelected(bool _value)
	{
		if (_value)
			setItemSelected();
	}*/

	void TabItem::setCaption(const UString& _value)
	{
		Tab* owner = getParent() != nullptr ? getParent()->castType<Tab>(false) : nullptr;
		if (owner != nullptr)
			owner->setItemName(this, _value);
		else
			Base::setCaption(_value);
	}

	const UString& TabItem::getCaption()
	{
		Tab* owner = getParent() != nullptr ? getParent()->castType<Tab>(false) : nullptr;
		if (owner != nullptr)
			return owner->getItemName(this);
		return Base::getCaption();
	}

	void TabItem::setButtonWidth(int _width)
	{
		Tab* owner = getParent() != nullptr ? getParent()->castType<Tab>(false) : nullptr;
		if (owner != nullptr)
			owner->setButtonWidth(this, _width);
	}

	/*int TabItem::getButtonWidth()
	{
		if (getParent() != nullptr)
			return getParent()->getButtonWidth(this);
		return 0;
	}

	const UString& TabItem::getItemName()
	{
		if (getParent() != nullptr)
			return getParent()->getItemName(this);
		//FIXME
		static UString empty;
		return empty;
	}

	void TabItem::setItemName(const UString& _name)
	{
		if (getParent() != nullptr)
			getParent()->setItemName(this, _name);
	}

	void TabItem::setItemData(Any _data)
	{
		if (getParent() != nullptr)
			getParent()->setItemData(this, _data);
	}

	void TabItem::setItemSelected()
	{
		if (getParent() != nullptr)
			(getParent()->setItemSelected(this);
	}

	void TabItem::removeItem()
	{
		if (getParent() != nullptr)
			getParent()->removeItem(this);
	}*/

	void TabItem::setProperty(const std::string& _key, const std::string& _value)
	{
		if (_key == "TabItem_ButtonWidth") setButtonWidth(utility::parseValue<int>(_value));
		/*else if (_key == "TabItem_Select") setSelected(utility::parseValue<bool>(_value));

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
#endif // MYGUI_DONT_USE_OBSOLETE*/

		else
		{
			Base::setProperty(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

} // namespace MyGUI
