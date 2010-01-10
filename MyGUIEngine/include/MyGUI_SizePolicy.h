/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
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
#ifndef __MYGUI_SIZE_POLICY_H__
#define __MYGUI_SIZE_POLICY_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	struct MYGUI_EXPORT SizePolicy
	{
		enum Enum
		{
			Manual,
			Content,
			ContentWidth,
			ContentHeight,
			MAX
		};

		static SizePolicy parse(const std::string& _value)
		{
			SizePolicy type;
			int value = 0;
			while (true)
			{
				const char * name = type.getValueName(value);
				if (strcmp(name, "") == 0 || name == _value) break;
				value++;
			}
			type.value = (Enum)value;
			return type;
		}

		SizePolicy() : value(Manual) { }
		SizePolicy(Enum _value) : value(_value) { }

		friend bool operator == (SizePolicy const& a, SizePolicy const& b) { return a.value == b.value; }
		friend bool operator != (SizePolicy const& a, SizePolicy const& b) { return a.value != b.value; }

		friend std::ostream& operator << ( std::ostream& _stream, const SizePolicy&  _value )
		{
			_stream << _value.getValueName(_value.value);
			return _stream;
		}

		friend std::istream& operator >> ( std::istream& _stream, SizePolicy&  _value )
		{
			std::string value;
			_stream >> value;
			_value = SizePolicy::parse(value);
			return _stream;
		}

		std::string print() const { return getValueName(value); }

	private:
		const char * getValueName(int _index) const
		{
			static const char * values[MAX + 1] = { "Manual", "Content", "ContentWidth", "ContentHeight", "" };
			return values[(_index < MAX && _index >= 0) ? _index : MAX];
		}

	private:
		Enum value;
	};

} // namespace MyGUI

#endif // __MYGUI_SIZE_POLICY_H__
