/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*/
#ifndef __MYGUI_WIDGET_TYPE_H__
#define __MYGUI_WIDGET_TYPE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	struct MYGUI_EXPORT WidgetStyle
	{
		enum Enum {
			Child, /**< child widget, cropped by parent widget borders, no overlapping (used by default for child widgets) */
			Popup, /**< popup widget, have parent widget, but not cropped on its borders */
			Overlapped,/**< child widget, cropped by parent widget borders, can overlap (used by default for root widgets) */
			MAX
		};

		static WidgetStyle parse(const std::string & _value)
		{
			WidgetStyle type;
			int value = 0;
			while (true) {
				const char * name = type.getValueName(value);
				if (strcmp(name, "") == 0 || name == _value) break;
				value++;
			};
			type.value = (Enum)value;
			return type;
		}

		WidgetStyle() : value(MAX) { }
		WidgetStyle(Enum _value) : value(_value) { }

		friend bool operator == (WidgetStyle const & a, WidgetStyle const & b) { return a.value == b.value; }
		friend bool operator != (WidgetStyle const & a, WidgetStyle const & b) { return a.value != b.value; }

		friend std::ostream& operator << ( std::ostream& _stream, const WidgetStyle &  _value ) {
			_stream << _value.getValueName(_value.value);
			return _stream;
		}

		friend std::istream& operator >> ( std::istream& _stream, WidgetStyle &  _value ) {
			std::string value;
			_stream >> value;
			_value = WidgetStyle::parse(value);
			return _stream;
		}

		std::string print() const { return getValueName(value); }
		Enum getValue() { return value; }

	private:
		const char * getValueName(int _index) const
		{
			static const char * values[MAX + 1] = { "Child", "Popup", "Overlapped", "" };
			return values[(_index < MAX && _index >= 0) ? _index : MAX];
		}

	private:
		Enum value;
	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_TYPE_H__
