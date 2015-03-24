/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_WIDGET_STYLE_H_
#define MYGUI_WIDGET_STYLE_H_

#include "MyGUI_Prerequest.h"
#include <string.h>

namespace MyGUI
{

	struct MYGUI_EXPORT WidgetStyle
	{
		enum Enum
		{
			Child, /**< child widget, cropped by parent widget borders, no overlapping (used by default for child widgets) */
			Popup, /**< popup widget, have parent widget, but not cropped on its borders */
			Overlapped, /**< child widget, cropped by parent widget borders, can overlap (used by default for root widgets) */
			MAX
		};

		WidgetStyle() :
			mValue(MAX)
		{
		}

		WidgetStyle(Enum _value) :
			mValue(_value)
		{
		}

		static WidgetStyle parse(const std::string& _value)
		{
			WidgetStyle type;
			int value = 0;
			while (true)
			{
				const char* name = type.getValueName(value);
				if (strcmp(name, "") == 0 || name == _value)
					break;
				value++;
			}
			type.mValue = (Enum)value;
			return type;
		}

		friend bool operator == (WidgetStyle const& a, WidgetStyle const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator != (WidgetStyle const& a, WidgetStyle const& b)
		{
			return a.mValue != b.mValue;
		}

		friend std::ostream& operator << (std::ostream& _stream, const WidgetStyle&  _value)
		{
			_stream << _value.getValueName(_value.mValue);
			return _stream;
		}

		friend std::istream& operator >> (std::istream& _stream, WidgetStyle&  _value)
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
			static const char* values[MAX + 1] = { "Child", "Popup", "Overlapped", "" };
			return values[(_index < MAX && _index >= 0) ? _index : MAX];
		}

	private:
		Enum mValue;
	};

} // namespace MyGUI

#endif // MYGUI_WIDGET_STYLE_H_
