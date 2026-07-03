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
#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wlifetime-safety-invalidation"
#endif
	inline void trim(std::string& _str, bool _left = true, bool _right = true)
	{
		if (_right)
			_str.erase(_str.find_last_not_of(" \t\r") + 1);
		if (_left)
			_str.erase(0, _str.find_first_not_of(" \t\r"));
	}
#if defined(__clang__)
	#pragma clang diagnostic pop
#endif

	template<typename T>
	inline std::string toString(T _value)
	{
		std::ostringstream stream;
		stream << _value;
		return stream.str();
	}

	inline const std::string& toString(const std::string& _value)
	{
		return _value;
	}

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
		std::stringstream stream;
		stream << _value;
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

	namespace templates
	{
		template<typename ReturnType, typename InputType = ReturnType>
		std::vector<ReturnType> split(const InputType& _source, const InputType& _delims)
		{
			std::vector<ReturnType> result;

			std::string_view source{_source};
			std::string_view delims{_delims};

			size_t start = source.find_first_not_of(delims);

			while (start != std::string_view::npos)
			{
				const auto end = source.find_first_of(delims, start);

				if (end == std::string_view::npos)
				{
					result.emplace_back(source.substr(start));
					break;
				}

				result.emplace_back(source.substr(start, end - start));
				start = source.find_first_not_of(delims, end + 1);
			}

			return result;
		}
	} // namespace templates

	inline std::vector<std::string> split(std::string_view _source, std::string_view _delims = "\t\n ")
	{
		return templates::split<std::string>(_source, _delims);
	}

	template<typename... Args>
	inline bool parseComplex(std::string_view _value, Args&... args)
	{
		std::stringstream stream;
		stream << _value;

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

	inline bool startWith(std::string_view _source, std::string_view _value)
	{
#if __cplusplus >= 202002L
		return _source.starts_with(_value);
#else
		size_t count = _value.size();
		if (_source.size() < count)
			return false;
		for (size_t index = 0; index < count; ++index)
		{
			if (_source[index] != _value[index])
				return false;
		}
		return true;
#endif
	}

	inline bool endWith(std::string_view _source, std::string_view _value)
	{
#if __cplusplus >= 202002L
		return _source.ends_with(_value);
#else
		size_t count = _value.size();
		if (_source.size() < count)
			return false;
		size_t offset = _source.size() - count;
		for (size_t index = 0; index < count; ++index)
		{
			if (_source[index + offset] != _value[index])
				return false;
		}
		return true;
#endif
	}

} // namespace MyGUI

#endif // MYGUI_STRING_UTILITY_H_
