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

	struct _MyGUIExport MenuItemType
	{
		typedef enum
		{
			Normal,
			Popup,
			Separator
		} Enum;

		MenuItemType() : value(Normal) { }
		MenuItemType(Enum _value) : value(_value) { }

		explicit MenuItemType(int _value) : value(_value) { }
		explicit MenuItemType(const std::string & _value) : value(MenuItemType::parse(_value).value) { }

		friend bool operator == (MenuItemType const & a, MenuItemType const & b) { return a.value == b.value; }
		friend bool operator != (MenuItemType const & a, MenuItemType const & b) { return a.value != b.value; }

        friend std::ostream& operator << ( std::ostream& _stream, const MenuItemType &  _value )
        {
			_stream << _value.print();
            return _stream;
        }

        friend std::istream& operator >> ( std::istream& _stream, MenuItemType &  _value )
        {
			std::string parse;
            _stream >> parse;
			if (_stream.fail()) _value = MenuItemType::Normal;
			else _value = MenuItemType::parse(parse);
            return _stream;
        }

		static MenuItemType parse(const std::string & _value);
		std::string print() const;

		int toValue() { return value; }

	private:
		int value;

		typedef std::map<std::string, MenuItemType> MapValue;

		static MapValue * getMapValue()
		{
			static MapValue map_value;
			if (map_value.empty()) {
			}
			return &map_value;
		}

		static MapValue mMapValue;
		static void initialise();
	};

} // namespace MyGUI

#endif // __MYGUI_MENU_ITEM_TYPE_H__
