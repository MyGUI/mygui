/*!
	@file
	@author		Albert Semenov
	@date		12/2010
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
#include "MyGUI_MultiListItem.h"
#include "MyGUI_MultiListBox.h"

namespace MyGUI
{

	MultiListItem::MultiListItem()
	{
	}

	void MultiListItem::initialiseOverride()
	{
		Base::initialiseOverride();
	}

	void MultiListItem::shutdownOverride()
	{
		Base::shutdownOverride();
	}

	void MultiListItem::setCaption(const UString& _value)
	{
		MultiListBox* owner = getOwner();
		if (owner != nullptr)
			owner->setColumnName(this, _value);
		else
			Base::setCaption(_value);
	}

	const UString& MultiListItem::getCaption()
	{
		MultiListBox* owner = getOwner();
		if (owner != nullptr)
			return owner->getColumnName(this);

		return Base::getCaption();
	}

	void MultiListItem::setItemResizingPolicy(ResizingPolicy _value)
	{
		MultiListBox* owner = getOwner();
		if (owner != nullptr)
			owner->setColumnResizingPolicy(this, _value);
	}

	void MultiListItem::setItemWidth(int _value)
	{
		MultiListBox* owner = getOwner();
		if (owner != nullptr)
			owner->setColumnWidth(this, _value);
	}

	void MultiListItem::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		if (_key == "ItemResizingPolicy")
			setItemResizingPolicy(ResizingPolicy::parse(_value));
		else if (_key == "ItemWidth")
			setItemWidth(utility::parseValue<int>(_value));
		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}
		eventChangeProperty(this, _key, _value);
	}

	MultiListBox* MultiListItem::getOwner()
	{
		if (getParent() != nullptr)
		{
			if (getParent()->isType<MultiListBox>())
				return getParent()->castType<MultiListBox>();
			else if ((getParent()->getParent() != nullptr) && (getParent()->getParent()->getClientWidget() == getParent()))
			{
				if (getParent()->getParent()->isType<MultiListBox>())
					return getParent()->getParent()->castType<MultiListBox>();
			}
		}

		return nullptr;
	}

} // namespace MyGUI
