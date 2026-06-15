/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_FLOW_DIRECTION_H_
#define MYGUI_FLOW_DIRECTION_H_

#include "MyGUI_Prerequest.h"
#include <string>
#include <string.h>
#include <iostream>

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

		static FlowDirection parse(std::string_view _value)
		{
			FlowDirection type;
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

		bool isHorizontal() const
		{
			return mValue == LeftToRight || mValue == RightToLeft;
		}

		bool isVertical() const
		{
			return !isHorizontal();
		}

		friend bool operator==(FlowDirection const& a, FlowDirection const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator!=(FlowDirection const& a, FlowDirection const& b)
		{
			return a.mValue != b.mValue;
		}

		friend std::ostream& operator<<(std::ostream& _stream, const FlowDirection& _value)
		{
			_stream << _value.getValueName(_value.mValue);
			return _stream;
		}

		friend std::istream& operator>>(std::istream& _stream, FlowDirection& _value)
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
			static const std::string_view values[MAX] = {"LeftToRight", "RightToLeft", "TopToBottom", "BottomToTop"};
			return values[_index];
		}

	private:
		Enum mValue;
	};

} // namespace MyGUI

#endif // MYGUI_FLOW_DIRECTION_H_
