/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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
		/// @wproperty{MultiListItem, ItemResizingPolicy, ResizingPolicy} Поведение при изменении размера.
		if (_key == "ItemResizingPolicy")
			setItemResizingPolicy(ResizingPolicy::parse(_value));

		/// @wproperty{MultiListItem, ItemWidth, int} Ширина списка.
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
