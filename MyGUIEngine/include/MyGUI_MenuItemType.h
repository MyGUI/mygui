/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_MENU_ITEM_TYPE_H_
#define MYGUI_MENU_ITEM_TYPE_H_

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	struct MYGUI_EXPORT MenuItemType
	{
		enum Enum
		{
			Normal,
			Popup,
			Separator,
			MAX
		};

		MenuItemType(Enum _value = MAX) :
			mValue(_value)
		{
		}

		static MenuItemType parse(std::string_view _value)
		{
			MenuItemType type;
			int value = 0;
			while (true)
			{
				std::string_view name = type.getValueName(value);
				if (name.empty() || name == _value)
					break;
				value++;
			}
			type.mValue = static_cast<Enum>(value);
			return type;
		}

		friend bool operator==(MenuItemType const& a, MenuItemType const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator!=(MenuItemType const& a, MenuItemType const& b)
		{
			return a.mValue != b.mValue;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const MenuItemType& _value)
		{
			_stream << _value.getValueName(_value.mValue);
			return _stream;
		}

		friend std::istream& operator>>(std::istream& _stream, MenuItemType& _value)
		{
			std::string value;
			_stream >> value;
			_value = parse(value);
			return _stream;
		}

		std::string_view print() const
		{
			return getValueName(mValue);
		}

		int getValue() const
		{
			return mValue;
		}

	private:
		std::string_view getValueName(int _index) const
		{
			if (_index < 0 || _index >= MAX)
				return {};
			static const std::string_view values[MAX] = {"Normal", "Popup", "Separator"};
			return values[_index];
		}

	private:
		Enum mValue;
	};

} // namespace MyGUI

#endif // MYGUI_MENU_ITEM_TYPE_H_
