/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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
