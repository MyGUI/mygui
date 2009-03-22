/*!
	@file
	@author		Albert Semenov
	@date		12/2008
	@module
*//*
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

#include "MyGUI_Precompiled.h"
#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{
	namespace demonstrate
	{

		// этот класс не для использования, а для демонстрации оберток для типов
		// обертка позваляет ограничить неявные действия с типом, ограничить видимость,
		// а так же поддержка парсинга и вывода типа в строку
		/** This class not for use. It's for demonstration of wrapper for types. Wrapper gives you possibility to restrict implicit actions with type, to restrict visibility and support of type parsing and serialising to string.*/
		struct StateType
		{
			enum Enum
			{
				Disabled,
				Normal,
				Pushed,
				MAX
			};

			static StateType parse(const std::string & _value)
			{
				StateType type;
				int value = 0;
				while (true)
				{
					const char * name = type.getValueName(value);
					if (strcmp(name, "") == 0 || name == _value) break;
					value++;
				};
				type.value = Enum(value);
				return type;
			}

			StateType(Enum _value = MAX) : value(_value) { }

			friend bool operator == (StateType const & a, StateType const & b) { return a.value == b.value; }
			friend bool operator != (StateType const & a, StateType const & b) { return a.value != b.value; }

			friend std::ostream& operator << ( std::ostream& _stream, const StateType &  _value )
			{
				_stream << _value.getValueName(_value.value);
				return _stream;
			}

			friend std::istream& operator >> ( std::istream& _stream, StateType &  _value )
			{
				std::string value;
				_stream >> value;
				_value = StateType::parse(value);
				return _stream;
			}

			std::string print() const { return getValueName(value); }

		private:
			const char * getValueName(int _index) const
			{
				static const char * values[MAX + 1] = { "Disabled", "Normal", "Pushed", "" };
				return values[(_index < MAX && _index >= 0) ? _index : MAX];
			}

		private:
			Enum value;
		};



		// example
		void test()
		{
			StateType type1 = StateType::Normal;
			StateType type2(StateType::Pushed);

			if (type1 == type2) { }
			if (type2 != type1) { }
			if (type1 == StateType::Disabled) { }
			if (StateType::Normal == type2) { }

			type1 = type2;
			type2 = StateType::Normal;

			StateType type3 = StateType::parse("Disabled");
			std::string name = type3.print();
		}

	} // namespace demonstrate
} // namespace MyGUI
