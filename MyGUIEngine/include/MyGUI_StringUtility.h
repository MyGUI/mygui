/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_STRING_UTILITY_H_
#define MYGUI_STRING_UTILITY_H_

#include "MyGUI_Prerequest.h"
#include <vector>
#include <sstream>

namespace MyGUI::utility
{

	inline void trim(std::string& _str, bool _left = true, bool _right = true)
	{
		if (_right) _str.erase(_str.find_last_not_of(" \t\r") + 1);
		if (_left) _str.erase(0, _str.find_first_not_of(" \t\r"));
	}

	// конвертирование в строку
	template<typename T>
	inline std::string toString (T p)
	{
		std::ostringstream stream;
		stream << p;
		return stream.str();
	}

	inline const std::string& toString (const std::string& _value)
	{
		return _value;
	}

	template<>
	inline std::string toString (std::string_view _value)
	{
		return std::string{ _value };
	}

	template<typename T1,  typename T2>
	inline std::string toString (T1 p1, T2 p2)
	{
		std::ostringstream stream;
		stream << p1 << p2;
		return stream.str();
	}

	template<typename T1,  typename T2,  typename T3>
	inline std::string toString (T1 p1, T2 p2, T3 p3)
	{
		std::ostringstream stream;
		stream << p1 << p2 << p3;
		return stream.str();
	}

	template<typename T1,  typename T2,  typename T3, typename T4>
	inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4)
	{
		std::ostringstream stream;
		stream << p1 << p2 << p3 << p4;
		return stream.str();
	}

	template<typename T1,  typename T2,  typename T3, typename T4, typename T5>
	inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		std::ostringstream stream;
		stream << p1 << p2 << p3 << p4 << p5;
		return stream.str();
	}

	template<typename T1,  typename T2,  typename T3, typename T4, typename T5, typename T6>
	inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		std::ostringstream stream;
		stream << p1 << p2 << p3 << p4 << p5 << p6;
		return stream.str();
	}

	template<typename T1,  typename T2,  typename T3, typename T4, typename T5, typename T6, typename T7>
	inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		std::ostringstream stream;
		stream << p1 << p2 << p3 << p4 << p5 << p6 << p7;
		return stream.str();
	}

	template<typename T1,  typename T2,  typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
		std::ostringstream stream;
		stream << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8;
		return stream.str();
	}

	template<typename T1,  typename T2,  typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
	inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8, T9 p9)
	{
		std::ostringstream stream;
		stream << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8 << p9;
		return stream.str();
	}

	template<>
	inline std::string toString<bool> (bool _value)
	{
		return _value ? "true" : "false";
	}


	// утилиты для парсинга
	template<typename T>
	inline T parseValue( std::string_view _value )
	{
		std::stringstream stream;
		stream << _value;
		T result;
		stream >> result;
		if (stream.fail())
			return T();
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return T();
				item = stream.get();
			}
		}
		return result;
	}

	// отдельная имплементация под bool
	template<>
	inline bool parseValue(std::string_view _value)
	{
		return _value == "True" || _value == "true" || _value == "1";
	}

	// отдельная имплементация под char
	template<>
	inline char parseValue(std::string_view _value)
	{
		return static_cast<char>(parseValue<short>(_value));
	}

	// отдельная имплементация под unsigned char
	template<>
	inline unsigned char parseValue(std::string_view _value)
	{
		return static_cast<unsigned char>(parseValue<unsigned short>(_value));
	}


	inline short parseShort(std::string_view _value)
	{
		return parseValue<short>(_value);
	}

	inline unsigned short parseUShort(std::string_view _value)
	{
		return parseValue<unsigned short>(_value);
	}

	inline int parseInt(std::string_view _value)
	{
		return parseValue<int>(_value);
	}

	inline unsigned int parseUInt(std::string_view _value)
	{
		return parseValue<unsigned int>(_value);
	}

	inline size_t parseSizeT(std::string_view _value)
	{
		return parseValue<size_t>(_value);
	}

	inline float parseFloat(std::string_view _value)
	{
		return parseValue<float>(_value);
	}

	inline double parseDouble(std::string_view _value)
	{
		return parseValue<double>(_value);
	}

	inline bool parseBool(std::string_view _value)
	{
		return parseValue<bool>(_value);
	}

	inline char parseChar(std::string_view _value)
	{
		return parseValue<char>(_value);
	}

	inline unsigned char parseUChar(std::string_view _value)
	{
		return parseValue<unsigned char>(_value);
	}

	// для парсинга сложных типов, состоящих из простых
	template<typename T1, typename T2>
	inline T1 parseValueEx2(std::string_view _value)
	{
		T2 p1, p2;
		std::stringstream stream;
		stream << _value;
		stream >> p1 >> p2;
		if (stream.fail())
			return T1();
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return T1();
				item = stream.get();
			}
		}
		return T1(p1, p2);
	}

	template<typename T1, typename T2>
	inline T1 parseValueEx3(std::string_view _value)
	{
		T2 p1, p2, p3;
		std::stringstream stream;
		stream << _value;
		stream >> p1 >> p2 >> p3;
		if (stream.fail())
			return T1();
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return T1();
				item = stream.get();
			}
		}
		return T1(p1, p2, p3);
	}

	template<typename T1, typename T2>
	inline T1 parseValueEx4(std::string_view _value)
	{
		T2 p1, p2, p3, p4;
		std::stringstream stream;
		stream << _value;
		stream >> p1 >> p2 >> p3 >> p4;
		if (stream.fail())
			return T1();
		else
		{
			int item = stream.get();
			while (item != -1)
			{
				if (item != ' ' && item != '\t')
					return T1();
				item = stream.get();
			}
		}
		return T1(p1, p2, p3, p4);
	}

	namespace templates
	{
		template<class ReturnType, class InputType = ReturnType>
		inline void split(std::vector<ReturnType>& _ret, const InputType& _source, const InputType& _delims)
		{
			size_t start = _source.find_first_not_of(_delims);
			while (start != _source.npos)
			{
				size_t end = _source.find_first_of(_delims, start);
				if (end != _source.npos)
					_ret.emplace_back(_source.substr(start, end - start));
				else
				{
					_ret.emplace_back(_source.substr(start));
					break;
				}
				start = _source.find_first_not_of(_delims, end + 1);
			}
		}
	} // namespace templates

	inline std::vector<std::string> split(std::string_view _source, std::string_view _delims = "\t\n ")
	{
		std::vector<std::string> result;
		templates::split<std::string, std::string_view>(result, _source, _delims);
		return result;
	}

	template<typename T1, typename T2, typename T3, typename T4>
	inline bool parseComplex(std::string_view _value, T1& _p1, T2& _p2, T3& _p3, T4& _p4)
	{
		std::stringstream stream;
		stream << _value;

		stream >> _p1 >> _p2 >> _p3 >> _p4;

		if (stream.fail())
			return false;
		int item = stream.get();
		while (item != -1)
		{
			if (item != ' ' && item != '\t')
				return false;
			item = stream.get();
		}

		return true;
	}

	template<typename T1, typename T2, typename T3>
	inline bool parseComplex(std::string_view _value, T1& _p1, T2& _p2, T3& _p3)
	{
		std::stringstream stream;
		stream << _value;

		stream >> _p1 >> _p2 >> _p3;

		if (stream.fail())
			return false;
		int item = stream.get();
		while (item != -1)
		{
			if (item != ' ' && item != '\t')
				return false;
			item = stream.get();
		}

		return true;
	}

	template<typename T1, typename T2>
	inline bool parseComplex(std::string_view _value, T1& _p1, T2& _p2)
	{
		std::stringstream stream;
		stream << _value;

		stream >> _p1 >> _p2;

		if (stream.fail())
			return false;
		int item = stream.get();
		while (item != -1)
		{
			if (item != ' ' && item != '\t')
				return false;
			item = stream.get();
		}

		return true;
	}

	template<typename T1>
	inline bool parseComplex(std::string_view _value, T1& _p1)
	{
		std::stringstream stream;
		stream << _value;

		stream >> _p1;

		if (stream.fail())
			return false;
		int item = stream.get();
		while (item != -1)
		{
			if (item != ' ' && item != '\t')
				return false;
			item = stream.get();
		}

		return true;
	}

	template<>
	inline bool parseComplex<bool>(std::string_view _value, bool& _p1)
	{
		std::string value(_value);
		trim(value);
		if ((value == "True") || (value == "true") || (value == "1"))
		{
			_p1 = true;
			return true;
		}
		else if ((value == "False") || (value == "false") || (value == "0"))
		{
			_p1 = false;
			return true;
		}

		return false;
	}

	inline bool startWith(std::string_view _source, std::string_view _value)
	{
#if __cplusplus >= 202002L
		return _source.starts_with(_value);
#endif
		size_t count = _value.size();
		if (_source.size() < count)
			return false;
		for (size_t index = 0; index < count; ++ index)
		{
			if (_source[index] != _value[index])
				return false;
		}
		return true;
	}

	inline bool endWith(std::string_view _source, std::string_view _value)
	{
#if __cplusplus >= 202002L
		return _source.ends_with(_value);
#endif
		size_t count = _value.size();
		if (_source.size() < count)
			return false;
		size_t offset = _source.size() - count;
		for (size_t index = 0; index < count; ++ index)
		{
			if (_source[index + offset] != _value[index])
				return false;
		}
		return true;
	}

} // namespace MyGUI

#endif // MYGUI_STRING_UTILITY_H_
