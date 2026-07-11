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
	MYGUI_EXPORT void trim(std::string& _str, bool _left = true, bool _right = true);

	template<typename T>
	inline std::string toString(T _value)
	{
		std::ostringstream stream;
		stream << _value;
		return stream.str();
	}

	MYGUI_EXPORT const std::string& toString(const std::string& _value);

	template<>
	inline std::string toString(std::string_view _value)
	{
		return std::string{_value};
	}

	template<typename... Args>
	inline std::string toString(Args&&... args)
	{
		std::ostringstream stream;
		((stream << args), ...);
		return stream.str();
	}

	template<>
	inline std::string toString<bool>(bool _value)
	{
		return _value ? "true" : "false";
	}


	template<typename T>
	inline T parseValue(std::string_view _value)
	{
		std::stringstream stream(std::string{_value});
		T result;
		stream >> result;
		if (stream.fail())
			return {};

		// check if there is more data, return {} in this case
		int item = stream.get();
		while (item != -1)
		{
			if (item != ' ' && item != '\t')
				return {};
			item = stream.get();
		}
		return result;
	}

	// bool specialization
	template<>
	inline bool parseValue(std::string_view _value)
	{
		return _value == "True" || _value == "true" || _value == "1";
	}

	// char specialization
	template<>
	inline char parseValue(std::string_view _value)
	{
		return static_cast<char>(parseValue<short>(_value));
	}

	// unsigned char specialization
	template<>
	inline unsigned char parseValue(std::string_view _value)
	{
		return static_cast<unsigned char>(parseValue<unsigned short>(_value));
	}


	MYGUI_EXPORT int parseInt(std::string_view _value);
	MYGUI_EXPORT unsigned int parseUInt(std::string_view _value);
	MYGUI_EXPORT size_t parseSizeT(std::string_view _value);
	MYGUI_EXPORT float parseFloat(std::string_view _value);
	MYGUI_EXPORT double parseDouble(std::string_view _value);
	MYGUI_EXPORT bool parseBool(std::string_view _value);

	MYGUI_EXPORT std::vector<std::string> split(std::string_view _source, std::string_view _delims = "\t\n ");

	template<typename... Args>
	inline bool parseComplex(std::string_view _value, Args&... args)
	{
		std::stringstream stream(std::string{_value});

		((stream >> args), ...);

		if (stream.fail())
			return false;

		// check if there is more data, return false in this case
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
	inline bool parseComplex<bool>(std::string_view _value, bool& arg)
	{
		std::string value(_value);
		trim(value);
		if ((value == "True") || (value == "true") || (value == "1"))
		{
			arg = true;
			return true;
		}
		if ((value == "False") || (value == "false") || (value == "0"))
		{
			arg = false;
			return true;
		}

		return false;
	}

	MYGUI_EXPORT bool startWith(std::string_view _source, std::string_view _value);
	MYGUI_EXPORT bool endWith(std::string_view _source, std::string_view _value);

} // namespace MyGUI

#endif // MYGUI_STRING_UTILITY_H_
