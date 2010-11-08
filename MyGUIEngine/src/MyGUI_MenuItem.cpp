/*!
	@file
	@author		Albert Semenov
	@date		11/2008
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
#include "MyGUI_MenuItem.h"

namespace MyGUI
{

	MenuItem::MenuItem() :
		mOwner(nullptr)
	{
	}

	void MenuItem::initialiseOverride()
	{
		Base::initialiseOverride();

		// FIXME проверить смену скина ибо должно один раз вызываться
		Widget* parent = getParent();
		MYGUI_ASSERT(parent, "MenuItem must have parent MenuCtrl");
		if (!parent->isType<MenuCtrl>())
		{
			Widget* client = parent;
			parent = client->getParent();
			MYGUI_ASSERT(parent, "MenuItem must have parent MenuCtrl");
			MYGUI_ASSERT(parent->getClientWidget() == client, "MenuItem must have parent MenuCtrl");
			MYGUI_ASSERT(parent->isType<MenuCtrl>(), "MenuItem must have parent MenuCtrl");
		}
		mOwner = parent->castType<MenuCtrl>();

		//FIXME нам нуженфокус клавы
		setNeedKeyFocus(true);
	}

	void MenuItem::shutdownOverride()
	{
		// FIXME проверить смену скина ибо должно один раз вызываться
		mOwner->_notifyDeleteItem(this);

		Base::shutdownOverride();
	}

	void MenuItem::onWidgetCreated(Widget* _widget)
	{
		Base::onWidgetCreated(_widget);

		MenuCtrl* child = _widget->castType<MenuCtrl>(false);
		if (child != nullptr)
		{
			mOwner->_wrapItemChild(this, child);
		}
	}

	void MenuItem::setCaption(const UString& _value)
	{
		Button::setCaption(_value);
		mOwner->_notifyUpdateName(this);
	}

	const UString& MenuItem::getItemName()
	{
		return mOwner->getItemName(this);
	}

	void MenuItem::setItemName(const UString& _value)
	{
		mOwner->setItemName(this, _value);
	}

	void MenuItem::setItemData(Any _data)
	{
		mOwner->setItemData(this, _data);
	}

	void MenuItem::removeItem()
	{
		mOwner->removeItem(this);
	}

	void MenuItem::setItemId(const std::string& _id)
	{
		mOwner->setItemId(this, _id);
	}

	const std::string& MenuItem::getItemId()
	{
		return mOwner->getItemId(this);
	}

	size_t MenuItem::getItemIndex()
	{
		return mOwner->getItemIndex(this);
	}

	MenuCtrl* MenuItem::createItemChild()
	{
		return mOwner->createItemChild(this);
	}

	void MenuItem::setItemType(MenuItemType _type)
	{
		mOwner->setItemType(this, _type);
	}

	MenuItemType MenuItem::getItemType()
	{
		return mOwner->getItemType(this);
	}

	void MenuItem::setItemChildVisible(bool _visible)
	{
		mOwner->setItemChildVisible(this, _visible);
	}

	MenuCtrl* MenuItem::getItemChild()
	{
		return mOwner->getItemChild(this);
	}

	void MenuItem::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		if (_key == "MenuItemId")
			setItemId(_value);
		else if (_key == "MenuItemType")
			setItemType(utility::parseValue<MenuItemType>(_value));
		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

	MenuCtrl* MenuItem::getMenuCtrlParent()
	{
		return mOwner;
	}

	IItemContainer* MenuItem::getItemContainer()
	{
		return mOwner;
	}

} // namespace MyGUI
