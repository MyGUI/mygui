/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_U_STRING_H_
#define MYGUI_U_STRING_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace MyGUI
{

	//! UTF-32 text with UTF-8 and native wide-string conversion.
	//! Positions, sizes, searches and iterators use Unicode code points, not grapheme clusters.
	//! Comparisons use code-point ordering, not locale-aware collation.
	//! Stored UTF-32 values are validated when exported to UTF-8 or native wide strings.
	class MYGUI_EXPORT UString
	{
	public:
		using code_point = char32_t;
		using utf32string = std::u32string;
		static constexpr std::size_t npos = std::u32string::npos;

		class MYGUI_EXPORT invalid_data : public std::runtime_error
		{
		public:
			using std::runtime_error::runtime_error;
		};

		UString() = default;
		UString(const UString& text) :
			mData(text.mData)
		{
		}
		UString(UString&&) noexcept = default;
		UString& operator=(const UString& text)
		{
			if (this != &text)
			{
				mData = text.mData;
				mUTF8.clear();
				mWide.clear();
			}
			return *this;
		}
		UString& operator=(UString&&) noexcept = default;

		//! Construct from a Unicode code point.
		explicit UString(Char character) :
			UString(1, character)
		{
		}

		//! Repeat a Unicode code point count times.
		UString(std::size_t count, Char character);

		//! UTF-8 input. Malformed sequences are replaced with U+FFFD.
		UString(const char* text) :
			UString(std::string_view(text))
		{
		}

		UString(const char* text, std::size_t count) :
			UString(count == 0 ? std::string_view() : std::string_view(text, count))
		{
		}

		UString(const std::string& text) :
			UString(std::string_view(text))
		{
		}

		explicit UString(std::string_view text);

		UString(const wchar_t* text) :
			UString(std::wstring(text))
		{
		}

		UString(const std::wstring& text);
		explicit UString(utf32string text);

		std::size_t size() const noexcept
		{
			return mData.size();
		}

		std::size_t length() const noexcept
		{
			return mData.size();
		}

		bool empty() const noexcept
		{
			return mData.empty();
		}

		void clear() noexcept
		{
			mData.clear();
		}

		UString substr(std::size_t index, std::size_t count = npos) const
		{
			return UString(mData.substr(index, count));
		}

		UString& assign(std::string_view text);

		UString& append(const UString& text)
		{
			mData.append(text.mData);
			return *this;
		}

		void push_back(code_point character)
		{
			mData.push_back(character);
		}

		code_point& operator[](std::size_t index)
		{
			return mData.at(index);
		}

		const code_point& operator[](std::size_t index) const
		{
			return mData.at(index);
		}

		std::u32string::iterator begin() noexcept
		{
			return mData.begin();
		}

		std::u32string::const_iterator begin() const noexcept
		{
			return mData.begin();
		}

		std::u32string::iterator end() noexcept
		{
			return mData.end();
		}

		std::u32string::const_iterator end() const noexcept
		{
			return mData.end();
		}

		std::size_t find(const UString& text, std::size_t index = 0) const
		{
			return mData.find(text.mData, index);
		}

		std::size_t find(code_point character, std::size_t index = 0) const
		{
			return mData.find(character, index);
		}

		std::size_t rfind(code_point character, std::size_t index = npos) const
		{
			return mData.rfind(character, index);
		}

		std::size_t rfind(const UString& text, std::size_t index = npos) const
		{
			return mData.rfind(text.mData, index);
		}

		std::size_t find_first_of(const UString& text, std::size_t index = 0) const
		{
			return mData.find_first_of(text.mData, index);
		}

		std::size_t find_last_of(const UString& text, std::size_t index = npos) const
		{
			return mData.find_last_of(text.mData, index);
		}

		const utf32string& asUTF32() const noexcept
		{
			return mData;
		}

		//! UTF-8 and wide buffers stay empty until requested; each is refreshed on every call.
		//! A reference survives conversion to a different encoding, but its contents are
		//! replaced by the next conversion to the same encoding. Returned pointers/views
		//! may be invalidated by that conversion, assignment, moving or destruction.
		//! Concurrent conversions on the same object require external synchronization.
		//! Invalid Unicode scalar values are reported as invalid_data exceptions.
		const std::string& asUTF8() const;
		const std::wstring& asWStr() const;

		const char* asUTF8_c_str() const
		{
			return asUTF8().c_str();
		}

		const wchar_t* asWStr_c_str() const
		{
			return asWStr().c_str();
		}

		operator std::string() const
		{
			return asUTF8();
		}

		operator std::string_view() const
		{
			return asUTF8();
		}

		operator std::wstring() const
		{
			return asWStr();
		}

		friend bool operator==(const UString& left, const UString& right)
		{
			return left.mData == right.mData;
		}

		friend bool operator!=(const UString& left, const UString& right)
		{
			return !(left == right);
		}

		friend bool operator<(const UString& left, const UString& right)
		{
			return left.mData < right.mData;
		}

		friend UString operator+(UString left, const UString& right)
		{
			left.append(right);
			return left;
		}

		friend std::ostream& operator<<(std::ostream& stream, const UString& text)
		{
			return stream << text.asUTF8();
		}

	private:
		std::u32string mData;
		mutable std::string mUTF8;
		mutable std::wstring mWide;
	};

} // namespace MyGUI

#endif // MYGUI_U_STRING_H_
