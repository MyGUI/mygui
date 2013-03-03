/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_MENU_ITEM_TYPE_H__
#define __MYGUI_MENU_ITEM_TYPE_H__

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

		static MenuItemType parse(const std::string& _value)
		{
			MenuItemType type;
			int value = 0;
			while (true)
			{
				const char* name = type.getValueName(value);
				if (strcmp(name, "") == 0 || name == _value)
					break;
				value++;
			}
			type.mValue = Enum(value);
			return type;
		}

		friend bool operator == (MenuItemType const& a, MenuItemType const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator != (MenuItemType const& a, MenuItemType const& b)
		{
			return a.mValue != b.mValue;
		}

		friend std::ostream& operator << (std::ostream& _stream, const MenuItemType&  _value)
		{
			_stream << _value.getValueName(_value.mValue);
			return _stream;
		}

		friend std::istream& operator >> (std::istream& _stream, MenuItemType&  _value)
		{
			std::string value;
			_stream >> value;
			_value = parse(value);
			return _stream;
		}

		std::string print() const
		{
			return getValueName(mValue);
		}

		int getValue() const
		{
			return mValue;
		}

	private:
		const char* getValueName(int _index) const
		{
			static const char* values[MAX + 1] = { "Normal", "Popup", "Separator", "" };
			return values[(_index < MAX && _index >= 0) ? _index : MAX];
		}

	private:
		Enum mValue;
	};

} // namespace MyGUI

#endif // __MYGUI_MENU_ITEM_TYPE_H__
