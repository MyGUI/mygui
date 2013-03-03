/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_FLOW_DIRECTION_H__
#define __MYGUI_FLOW_DIRECTION_H__

#include "MyGUI_Prerequest.h"
#include <string>

namespace MyGUI
{

	struct MYGUI_EXPORT FlowDirection
	{
		enum Enum
		{
			LeftToRight,
			RightToLeft,
			TopToBottom,
			BottomToTop,
			MAX
		};

		FlowDirection(Enum _value = LeftToRight) :
			mValue(_value)
		{
		}

		static FlowDirection parse(const std::string& _value)
		{
			FlowDirection type;
			int value = 0;
			while (true)
			{
				const char* name = type.getValueName(value);
				if (strcmp(name, "") == 0 || name == _value) break;
				value++;
			}
			type.mValue = (Enum)value;
			return type;
		}

		bool isHorizontal() const
		{
			return mValue == LeftToRight || mValue == RightToLeft;
		}

		bool isVertical() const
		{
			return !isHorizontal();
		}

		friend bool operator == (FlowDirection const& a, FlowDirection const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator != (FlowDirection const& a, FlowDirection const& b)
		{
			return a.mValue != b.mValue;
		}

		friend std::ostream& operator << ( std::ostream& _stream, const FlowDirection&  _value )
		{
			_stream << _value.getValueName(_value.mValue);
			return _stream;
		}

		friend std::istream& operator >> ( std::istream& _stream, FlowDirection&  _value )
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
			static const char* values[MAX + 1] = { "LeftToRight", "RightToLeft", "TopToBottom", "BottomToTop", "" };
			return values[(_index < MAX && _index >= 0) ? _index : MAX];
		}

	private:
		Enum mValue;
	};

} // namespace MyGUI

#endif // __MYGUI_FLOW_DIRECTION_H__
