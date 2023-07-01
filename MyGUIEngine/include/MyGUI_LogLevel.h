/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_LOG_LEVEL_H_
#define MYGUI_LOG_LEVEL_H_

#include "MyGUI_Prerequest.h"
#include <string>
#include <cstring>
#include <iostream>

namespace MyGUI
{

	struct MYGUI_EXPORT LogLevel
	{
		enum Enum
		{
			Info, // Информационное сообщение.
			Warning, // Несущественная проблема.
			Error, // Устранимая ошибка.
			Critical, // Неустранимая ошибка или сбой в работе приложения.
			MAX
		};

		LogLevel() :
			mValue(Info)
		{
		}

		LogLevel(Enum _value) :
			mValue(_value)
		{
		}

		static LogLevel parse(std::string_view _value)
		{
			LogLevel type;
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

		friend bool operator<(LogLevel const& a, LogLevel const& b)
		{
			return a.mValue < b.mValue;
		}

		friend bool operator>=(LogLevel const& a, LogLevel const& b)
		{
			return !(a < b);
		}

		friend bool operator>(LogLevel const& a, LogLevel const& b)
		{
			return (b < a);
		}

		friend bool operator<=(LogLevel const& a, LogLevel const& b)
		{
			return !(a > b);
		}

		friend bool operator==(LogLevel const& a, LogLevel const& b)
		{
			return !(a < b) && !(a > b);
		}

		friend bool operator!=(LogLevel const& a, LogLevel const& b)
		{
			return !(a == b);
		}

		friend std::ostream& operator<<(std::ostream& _stream, const LogLevel& _value)
		{
			_stream << _value.getValueName(_value.mValue);
			return _stream;
		}

		friend std::istream& operator>>(std::istream& _stream, LogLevel& _value)
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
			static const std::string_view values[MAX] = {"Info", "Warning", "Error", "Critical"};
			return values[_index];
		}

	private:
		Enum mValue;
	};

} // namespace MyGUI

#endif // MYGUI_LOG_LEVEL_H_
