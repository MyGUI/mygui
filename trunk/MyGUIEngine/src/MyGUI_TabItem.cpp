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

	TabItem::TabItem()
	{
	}

	void TabItem::initialiseOverride()
	{
		Base::initialiseOverride();

		// FIXME проверить смену скина ибо должно один раз вызываться
		//mOwner = getParent()->castType<TabControl>();
	}

	void TabItem::shutdownOverride()
	{
		TabControl* owner = getParent() != nullptr ? getParent()->castType<TabControl>(false) : nullptr;
		if (owner != nullptr)
			owner->_notifyDeleteItem(this);

		Base::shutdownOverride();
	}

	void TabItem::setCaption(const UString& _value)
	{
		TabControl* owner = getParent() != nullptr ? getParent()->castType<TabControl>(false) : nullptr;
		if (owner != nullptr)
			owner->setItemName(this, _value);
		else
			Base::setCaption(_value);
	}

	const UString& TabItem::getCaption()
	{
		TabControl* owner = getParent() != nullptr ? getParent()->castType<TabControl>(false) : nullptr;
		if (owner != nullptr)
			return owner->getItemName(this);
		return Base::getCaption();
	}

	void TabItem::setButtonWidth(int _width)
	{
		TabControl* owner = getParent() != nullptr ? getParent()->castType<TabControl>(false) : nullptr;
		if (owner != nullptr)
			owner->setButtonWidth(this, _width);
	}

	void TabItem::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		/// @wproperty{TabItem, ButtonWidth, int} Ширина кнопки.
		if (_key == "ButtonWidth")
			setButtonWidth(utility::parseValue<int>(_value));

		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

} // namespace MyGUI
