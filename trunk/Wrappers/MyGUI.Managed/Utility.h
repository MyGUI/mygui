/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <vcclr.h>
#include <string>

namespace string_utility
{

	std::string utf16_to_utf8(const std::wstring& _source);
	std::string utf16_to_ansi(const std::wstring& _source);
	std::wstring utf8_to_utf16(const std::string& _source);
	std::wstring ansi_to_utf16(const std::string& _source);

	inline std::string managed_to_ansi(System::String ^ _str)
	{
		cli::pin_ptr<const wchar_t> str = PtrToStringChars(_str);
		return utf16_to_ansi(str);
	}

	inline std::string managed_to_utf8(System::String ^ _str)
	{
		cli::pin_ptr<const wchar_t> str = PtrToStringChars(_str);
		return utf16_to_utf8(str);
	}

	inline std::wstring managed_to_utf16(System::String ^ _str)
	{
		cli::pin_ptr<const wchar_t> str = PtrToStringChars(_str);
		return std::wstring(str);
	}

	inline System::String ^ ansi_to_managed(const std::string& _str)
	{
		return gcnew System::String(_str.c_str(), 0, (int)_str.size(), System::Text::Encoding::ASCII);
	}

	inline System::String ^ utf16_to_managed(const std::wstring& _str)
	{
		return gcnew System::String(_str.c_str());
	}

	inline System::String ^ utf8_to_managed(const std::string& _str)
	{
		return gcnew System::String(_str.c_str(), 0, (int)_str.size(), System::Text::Encoding::UTF8);
	}

} // namespace string_utility
