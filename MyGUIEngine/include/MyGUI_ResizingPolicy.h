/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_RESIZING_POLICY_H__
#define __MYGUI_RESIZING_POLICY_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	struct MYGUI_EXPORT ResizingPolicy
	{
		enum Enum
		{
			Auto,
			Fixed,
			Fill,
			MAX
		};

		ResizingPolicy(Enum _value = MAX) :
			mValue(_value)
		{
		}

		static ResizingPolicy parse(const std::string& _value)
		{
			ResizingPolicy type;
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

		friend bool operator == (ResizingPolicy const& a, ResizingPolicy const& b)
		{
			return a.mValue == b.mValue;
		}

		friend bool operator != (ResizingPolicy const& a, ResizingPolicy const& b)
		{
			return a.mValue != b.mValue;
		}

		friend std::ostream& operator << (std::ostream& _stream, const ResizingPolicy&  _value)
		{
			_stream << _value.getValueName(_value.mValue);
			return _stream;
		}

		friend std::istream& operator >> (std::istream& _stream, ResizingPolicy&  _value)
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
			static const char* values[MAX + 1] = { "Auto", "Fixed", "Fill", "" };
			return values[(_index < MAX && _index >= 0) ? _index : MAX];
		}

	private:
		Enum mValue;
	};

} // namespace MyGUI

#endif // __MYGUI_RESIZING_POLICY_H__
