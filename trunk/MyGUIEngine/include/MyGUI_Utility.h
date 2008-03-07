/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_UTILITY_H__
#define __MYGUI_UTILITY_H__

#include "MyGUI_Prerequest.h"
//#include <iostream>
#include <sstream>

namespace MyGUI
{
	namespace utility
	{

		// конвертирование в строку
		template< class T >
		inline std::string toString (T p)
		{
			std::ostringstream stream;
			stream << p;
			return stream.str();
		}

		template< class T1,  class T2 >
		inline std::string toString (T1 p1, T2 p2)
		{
			std::ostringstream stream;
			stream << p1 << p2;
			return stream.str();
		}

		template< class T1,  class T2,  class T3 >
		inline std::string toString (T1 p1, T2 p2, T3 p3)
		{
			std::ostringstream stream;
			stream << p1 << p2 << p3;
			return stream.str();
		}

		template< class T1,  class T2,  class T3, class T4 >
		inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4)
		{
			std::ostringstream stream;
			stream << p1 << p2 << p3 << p4;
			return stream.str();
		}

		template< class T1,  class T2,  class T3, class T4, class T5 >
		inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
		{
			std::ostringstream stream;
			stream << p1 << p2 << p3 << p4 << p5;
			return stream.str();
		}

		template< class T1,  class T2,  class T3, class T4, class T5, class T6 >
		inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
		{
			std::ostringstream stream;
			stream << p1 << p2 << p3 << p4 << p5 << p6;
			return stream.str();
		}

		template< class T1,  class T2,  class T3, class T4, class T5, class T6, class T7 >
		inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
		{
			std::ostringstream stream;
			stream << p1 << p2 << p3 << p4 << p5 << p6 << p7;
			return stream.str();
		}

		template< class T1,  class T2,  class T3, class T4, class T5, class T6, class T7, class T8 >
		inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
		{
			std::ostringstream stream;
			stream << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8;
			return stream.str();
		}


		// утилиты для парсинга
		template< class T >
		inline T parseValue( const std::string& _value )
		{
			std::istringstream str(_value);
			T ret = 0;
			str >> ret;
			return ret;
		}

		inline char parseChar(const std::string& _value) {return static_cast<char>(parseValue<short>(_value));}
		inline unsigned char parseUChar(const std::string& _value) {return static_cast<unsigned char>(parseValue<unsigned short>(_value));}
		inline short parseShort(const std::string& _value) {return parseValue<short>(_value);}
		inline unsigned short parseUShort(const std::string& _value) {return parseValue<unsigned short>(_value);}
		inline int parseInt(const std::string& _value) {return parseValue<int>(_value);}
		inline unsigned int parseUInt(const std::string& _value) {return parseValue<unsigned int>(_value);}
		inline size_t parseSizeT(const std::string& _value) {return parseValue<size_t>(_value);}
		inline float parseFloat(const std::string& _value) {return parseValue<float>(_value);}
		inline double parseDouble(const std::string& _value) {return parseValue<double>(_value);}
		inline bool parseBool(const std::string& _value) {return ( (_value == "true") || (_value == "1") );}

		// для парсинга сложных типов, состоящих из простых
		template< class T1, class T2 >
		T1 parseValueEx2(const std::string & _value)
		{
			T2 p1, p2;
			std::istringstream str(_value);
			str >> p1 >> p2;
			if (str.fail()) return T1();
			return T1(p1, p2);
		}

		template< class T1, class T2 >
		T1 parseValueEx3(const std::string & _value)
		{
			T2 p1, p2, p3;
			std::istringstream str(_value);
			str >> p1 >> p2 >> p3;
			if (str.fail()) return T1();
			return T1(p1, p2, p3);
		}

		template< class T1, class T2 >
		T1 parseValueEx4(const std::string & _value)
		{
			T2 p1, p2, p3, p4;
			std::istringstream str(_value);
			str >> p1 >> p2 >> p3 >> p4;
			if (str.fail()) return T1();
			return T1(p1, p2, p3, p4);
		}

		namespace templates
		{
			template< class T>
			void split(std::vector<std::string> & _ret, const std::string & _source, const std::string & _delims)
			{
				size_t start = _source.find_first_not_of(_delims);
				while (start != _source.npos) {
					size_t end = _source.find_first_of(_delims, start);
					if (end != _source.npos) _ret.push_back(_source.substr(start, end-start));
					else {
						_ret.push_back(_source.substr(start));
						break;
					}
					start = _source.find_first_not_of(_delims, end + 1);
				};
			}
		} // namespace templates

		inline std::vector<std::string> split(const std::string & _source, const std::string & _delims = "\t\n ")
		{
			std::vector<std::string> ret;
			templates::split<void>(ret, _source, _delims);
			return ret;
		}

		inline void trim(std::string& _str, bool _left = true, bool _right = true)
		{
			if (_right) _str.erase(_str.find_last_not_of(" \t\r")+1);
			if (_left) _str.erase(0, _str.find_first_not_of(" \t\r"));
		}

	} // namespace utility

} // namespace MyGUI

#endif // __MYGUI_UTILITY_H__
