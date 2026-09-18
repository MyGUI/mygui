/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_FILE_SYSTEM_UTILITY_H_
#define MYGUI_FILE_SYSTEM_UTILITY_H_

#include "MyGUI_UString.h"
#include <filesystem>
#include <string>
#include <string_view>

namespace MyGUI::utility
{

	//! Convert UTF-8 bytes to a native filesystem path.
	inline std::filesystem::path toPath(std::string_view text)
	{
		if (text.empty())
			return {};

		return std::filesystem::u8path(text.data(), text.data() + text.size());
	}

	//! Convert null-terminated UTF-8 bytes to a native filesystem path.
	inline std::filesystem::path toPath(const char* text)
	{
		return toPath(std::string_view(text));
	}

	//! Convert UTF-8 bytes to a native filesystem path.
	inline std::filesystem::path toPath(const std::string& text)
	{
		return toPath(std::string_view(text));
	}

	//! Convert Unicode text to a native filesystem path.
	inline std::filesystem::path toPath(const UString& text)
	{
		return toPath(text.asUTF8());
	}

	//! Convert a filesystem path to UTF-8 bytes.
	inline std::string toUtf8(const std::filesystem::path& path)
	{
		const auto text = path.u8string();
		return {text.begin(), text.end()};
	}

} // namespace MyGUI::utility

#endif // MYGUI_FILE_SYSTEM_UTILITY_H_
