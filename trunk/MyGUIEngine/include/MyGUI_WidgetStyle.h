/*!
	@file
	@author		Albert Semenov
	@date		12/2008
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
#ifndef __MYGUI_WIDGET_STYLE_H__
#define __MYGUI_WIDGET_STYLE_H__

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

#endif // __MYGUI_WIDGET_STYLE_H__
