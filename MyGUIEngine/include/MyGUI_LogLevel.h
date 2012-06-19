/*!
	@file
	@author		Albert Semenov
	@date		04/2010
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
#ifndef __MYGUI_LOG_LEVEL_H__
#define __MYGUI_LOG_LEVEL_H__

#include "MyGUI_Prerequest.h"

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

		static LogLevel parse(const std::string& _value)
		{
			LogLevel type;
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

		friend bool operator < (LogLevel const& a, LogLevel const& b)
		{
			return a.mValue < b.mValue;
		}

		friend bool operator >= (LogLevel const& a, LogLevel const& b)
		{
			return !(a < b);
		}

		friend bool operator > (LogLevel const& a, LogLevel const& b)
		{
			return (b < a);
		}

		friend bool operator <= (LogLevel const& a, LogLevel const& b)
		{
			return !(a > b);
		}

		friend bool operator == (LogLevel const& a, LogLevel const& b)
		{
			return !(a < b) && !(a > b);
		}

		friend bool operator != (LogLevel const& a, LogLevel const& b)
		{
			return !(a == b);
		}

		friend std::ostream& operator << (std::ostream& _stream, const LogLevel&  _value)
		{
			_stream << _value.getValueName(_value.mValue);
			return _stream;
		}

		friend std::istream& operator >> (std::istream& _stream, LogLevel&  _value)
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
			static const char* values[MAX + 1] = { "Info", "Warning", "Error", "Critical", "" };
			return values[(_index < MAX && _index >= 0) ? _index : MAX];
		}

	private:
		Enum mValue;
	};

} // namespace MyGUI

#endif // __MYGUI_LOG_LEVEL_H__
