/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#ifndef __MYGUI_MENU_ITEM_TYPE_H__
#define __MYGUI_MENU_ITEM_TYPE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	struct MYGUI_EXPORT MenuItemType
	{
		typedef enum EnumType { Normal, Popup, Separator, MAX } Enum;

		static MenuItemType parse(const std::string & _value)
		{
			MenuItemType type;
			int value = 0;
			while (true) {
				const char * name = type.getValueName(value);
				if (strcmp(name, "") == 0 || name == _value) break;
				value++;
			};
			type.value = value;
			return type;
		}

		MenuItemType() : value(MAX) { }
		MenuItemType(Enum _value) : value(_value) { }
		explicit MenuItemType(int _value) : value(_value) { }

		friend bool operator == (MenuItemType const & a, MenuItemType const & b) { return a.value == b.value; }
		friend bool operator != (MenuItemType const & a, MenuItemType const & b) { return a.value != b.value; }

		friend std::ostream& operator << ( std::ostream& _stream, const MenuItemType &  _value ) {
			_stream << _value.getValueName(_value.value);
			return _stream;
		}

		friend std::istream& operator >> ( std::istream& _stream, MenuItemType &  _value ) {
			std::string value;
			_stream >> value;
			_value = MenuItemType::parse(value);
			return _stream;
		}

		std::string print() const { return getValueName(value); }
		int toValue() { return value; }

	private:
		const char * getValueName(int _index) const
		{
			static const char * values[MAX + 1] = { "Normal", "Popup", "Separator", "" };
			return values[(_index < MAX && _index >= 0) ? _index : MAX];
		}

	private:
		int value;
	};

} // namespace MyGUI

#endif // __MYGUI_MENU_ITEM_TYPE_H__
