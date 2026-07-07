/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_StringUtility.h"

namespace MyGUI::utility
{

#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wlifetime-safety-invalidation"
#endif
	void trim(std::string& _str, bool _left, bool _right)
	{
		if (_right)
			_str.erase(_str.find_last_not_of(" \t\r") + 1);
		if (_left)
			_str.erase(0, _str.find_first_not_of(" \t\r"));
	}
#if defined(__clang__)
	#pragma clang diagnostic pop
#endif

	const std::string& toString(const std::string& _value)
	{
		return _value;
	}

	int parseInt(std::string_view _value)
	{
		return parseValue<int>(_value);
	}

	unsigned int parseUInt(std::string_view _value)
	{
		return parseValue<unsigned int>(_value);
	}

	size_t parseSizeT(std::string_view _value)
	{
		return parseValue<size_t>(_value);
	}

	float parseFloat(std::string_view _value)
	{
		return parseValue<float>(_value);
	}

	double parseDouble(std::string_view _value)
	{
		return parseValue<double>(_value);
	}

	bool parseBool(std::string_view _value)
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

	std::vector<std::string> split(std::string_view _source, std::string_view _delims)
	{
		return templates::split<std::string>(_source, _delims);
	}

	bool startWith(std::string_view _source, std::string_view _value)
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

	bool endWith(std::string_view _source, std::string_view _value)
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
