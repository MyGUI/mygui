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

		void validateCharacter(char32_t character)
		{
			char buffer[4];
			utf8::append(character, buffer);
		}

	}

	UString::UString(std::size_t count, Char character) :
		mData(count, static_cast<char32_t>(character))
	{
	}

	UString::UString(std::string_view text)
	{
		assign(text);
	}

	UString::UString(utf32string text) :
		mData(std::move(text))
	{
	}

	UString::UString(const std::wstring& text)
	{
		static_assert(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4);
		mData.reserve(text.size());
		for (auto iter = text.begin(); iter != text.end();)
		{
			char32_t character = U'\uFFFD';
			if constexpr (sizeof(wchar_t) == 2)
			{
				auto next = iter;
				try
				{
					const auto value = utf8::next16(next, text.end());
					// Some malformed sequences leave next unchanged instead of throwing.
					if (next != iter)
						character = value;
				}
				catch (const utf8::exception&)
				{
					next = iter;
				}
				// Replace only the unpaired surrogate; preserve the following character.
				iter = next == iter ? iter + 1 : next;
			}
			else
			{
				const auto value = static_cast<char32_t>(*iter++);
				try
				{
					validateCharacter(value);
					character = value;
				}
				catch (const utf8::exception&)
				{
					// Native UTF-32 input follows the same replacement policy.
				}
			}
			mData.push_back(character);
		}
	}

	UString& UString::assign(std::string_view text)
	{
		// An empty string_view may have a null data pointer.
		if (text.empty())
			mData.clear();
		else
			mData = convertText([text] { return utf8::utf8to32(utf8::replace_invalid(text)); });
		return *this;
	}

	const std::string& UString::asUTF8() const
	{
		mUTF8 = convertText([this] { return utf8::utf32to8(mData); });
		return mUTF8;
	}

	std::wstring UString::asWStr() const
	{
		return convertText(
			[this]
			{
				std::wstring result;
				result.reserve(mData.size());
				for (auto character : mData)
				{
					if constexpr (sizeof(wchar_t) == 2)
						utf8::append16(character, std::back_inserter(result));
					else
					{
						validateCharacter(character);
						result.push_back(static_cast<wchar_t>(character));
					}
				}
				return result;
			});
	}

} // namespace MyGUI
