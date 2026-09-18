/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_FILE_SYSTEM_UTILITY_H_
#define MYGUI_FILE_SYSTEM_UTILITY_H_

#include <filesystem>
#include <string>
#include <string_view>

namespace MyGUI::utility
{

	//! Convert UTF-8 bytes to a native filesystem path.
	inline std::filesystem::path pathFromUTF8(std::string_view text)
	{
		if (text.empty())
			return {};

		return std::filesystem::u8path(text.data(), text.data() + text.size());
	}

	//! Convert a filesystem path to UTF-8 bytes.
	inline std::string pathToUTF8(const std::filesystem::path& path)
	{
		const auto text = path.u8string();
		return {text.begin(), text.end()};
	}

} // namespace MyGUI::utility

#endif // MYGUI_FILE_SYSTEM_UTILITY_H_
