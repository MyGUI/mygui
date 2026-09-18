/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_UString.h"

#if defined(_MSVC_LANG)
	// MSVC might reports an old __cplusplus value
	#define UTF_CPP_CPLUSPLUS _MSVC_LANG
#endif
#include <utf8.h>

namespace MyGUI
{
	namespace
	{

		// Keep dependency exceptions out of the public API.
		template<typename Convert>
		auto convertText(Convert convert)
		{
			try
			{
				return convert();
			}
			catch (const utf8::exception& error)
			{
				throw UString::invalid_data(error.what());
			}
		}

	}

	UString::UString(std::size_t count, Char character)
	{
		convertText(
			[&]
			{
				mData.reserve(count);
				for (std::size_t index = 0; index < count; ++index)
					utf8::append16(character, std::back_inserter(mData));
			});
	}

	UString::UString(std::string_view text)
	{
		assign(text);
	}

	UString::UString(const utf32string& text) :
		mData(convertText(
			[&text]
			{
				std::u16string result;
				result.reserve(text.size());
				for (auto character : text)
					utf8::append16(character, std::back_inserter(result));
				return result;
			}))
	{
	}

	UString::UString(const std::wstring& text)
	{
		static_assert(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4);
		if constexpr (sizeof(wchar_t) == 2)
			mData.assign(text.begin(), text.end());
		else
			mData = UString(utf32string(text.begin(), text.end())).mData;
	}

	UString& UString::assign(std::string_view text)
	{
		// An empty string_view may have a null data pointer.
		if (text.empty())
			mData.clear();
		else
			mData = convertText([text] { return utf8::utf8to16(utf8::replace_invalid(text)); });
		return *this;
	}

	const std::string& UString::asUTF8() const
	{
		mUTF8 = convertText([this] { return utf8::utf16to8(mData); });
		return mUTF8;
	}

	const UString::utf32string& UString::asUTF32() const
	{
		// Use the checked bulk conversions without changing the UTF-8 result buffer.
		mUTF32 = convertText([this] { return utf8::utf8to32(utf8::utf16to8(mData)); });
		return mUTF32;
	}

	const std::wstring& UString::asWStr() const
	{
		if constexpr (sizeof(wchar_t) == 2)
		{
			// Validate raw UTF-16 edits before exporting the native wide string.
			convertText([this] { return utf8::utf16to8(mData); });
			mWide.assign(mData.begin(), mData.end());
		}
		else
		{
			const auto text = convertText([this] { return utf8::utf8to32(utf8::utf16to8(mData)); });
			mWide.assign(text.begin(), text.end());
		}
		return mWide;
	}

} // namespace MyGUI
