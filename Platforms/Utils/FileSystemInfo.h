#ifndef FILE_SYSTEM_INFO_H_
#define FILE_SYSTEM_INFO_H_

#include <MyGUI.h>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <cwctype>
#include <utility>
#include <limits>
#include "MyGUI_FileSystemUtility.h"

namespace common
{

	struct FileInfo
	{
		FileInfo(std::filesystem::path _name, bool _folder) :
			name(std::move(_name)),
			folder(_folder)
		{
		}
		std::filesystem::path name;
		bool folder;
	};
	using VectorFileInfo = std::vector<FileInfo>;
	using VectorPath = std::vector<std::filesystem::path>;

	inline bool matchWildcard(const MyGUI::UString& pattern, const MyGUI::UString& text)
	{
		size_t pi = 0, si = 0, starPi = MyGUI::UString::npos, starSi = 0;
		while (si < text.size())
		{
			if (pi < pattern.size() && (pattern[pi] == text[si] || pattern[pi] == U'?'))
			{
				++pi;
				++si;
			}
			else if (pi < pattern.size() && pattern[pi] == U'*')
			{
				starPi = pi++;
				starSi = si;
			}
			else if (starPi != MyGUI::UString::npos)
			{
				pi = starPi + 1;
				si = ++starSi;
			}
			else
				return false;
		}
		while (pi < pattern.size() && pattern[pi] == U'*')
			++pi;
		return pi == pattern.size();
	}

	inline VectorFileInfo getSystemFileList(
		const std::filesystem::path& _folder,
		const std::filesystem::path& _mask,
		bool _sorted = true)
	{
		std::error_code ec;

		const auto searchDir = _folder / _mask.parent_path();

		const MyGUI::UString pattern =
			_mask.filename() == "*.*" ? MyGUI::UString{} : MyGUI::UString(MyGUI::utility::toUtf8(_mask.filename()));

		auto iter = std::filesystem::directory_iterator(
			searchDir,
			std::filesystem::directory_options::skip_permission_denied,
			ec);
		if (ec)
		{
			MYGUI_LOG(Error, "Can't open " + MyGUI::utility::toUtf8(searchDir));
			return {};
		}

		VectorFileInfo result;

		// ".." is not returned by directory_iterator; add it manually so file dialogs can navigate up
		result.emplace_back("..", true);

		for (const auto& entry : iter)
		{
			auto name = entry.path().filename();

			if (!pattern.empty() && !matchWildcard(pattern, MyGUI::UString(MyGUI::utility::toUtf8(name))))
				continue;

			result.emplace_back(name, entry.is_directory());
		}

		if (_sorted)
			std::sort(
				result.begin(),
				result.end(),
				[](const FileInfo& a, const FileInfo& b)
				{
					if (a.folder != b.folder)
						return a.folder;
					const MyGUI::UString aName(MyGUI::utility::toUtf8(a.name));
					const MyGUI::UString bName(MyGUI::utility::toUtf8(b.name));
					return std::lexicographical_compare(
						aName.begin(),
						aName.end(),
						bName.begin(),
						bName.end(),
						[](char32_t c1, char32_t c2)
						{
							// Preserve code points outside the native wide character range.
							const auto lower1 = c1 > static_cast<char32_t>(std::numeric_limits<wchar_t>::max())
								? c1
								: static_cast<char32_t>(std::towlower(static_cast<wint_t>(c1)));
							const auto lower2 = c2 > static_cast<char32_t>(std::numeric_limits<wchar_t>::max())
								? c2
								: static_cast<char32_t>(std::towlower(static_cast<wint_t>(c2)));
							return lower1 < lower2;
						});
				});
		return result;
	}

	inline void scanFolder(
		VectorPath& _result,
		const std::filesystem::path& _folder,
		bool _recursive,
		const MyGUI::UString& _mask,
		bool _fullpath)
	{
		// Normalize DOS-style *.* (match all)
		const MyGUI::UString pattern = (_mask == "*.*") ? MyGUI::UString() : _mask;

		std::error_code ec;

		auto iterate = [&](auto iter, auto end) -> void
		{
			for (; iter != end; ++iter)
			{
				const auto& entry = *iter;
				auto name = entry.path().filename();
				if (entry.is_directory())
					continue;

				if (!pattern.empty() && !matchWildcard(pattern, MyGUI::UString(MyGUI::utility::toUtf8(name))))
					continue;

				if (_fullpath)
					_result.push_back(entry.path());
				else
					_result.push_back(std::move(name));
			}
		};

		if (_recursive)
		{
			auto iter = std::filesystem::recursive_directory_iterator(
				_folder,
				std::filesystem::directory_options::skip_permission_denied,
				ec);
			if (ec)
				return;
			iterate(iter, std::filesystem::recursive_directory_iterator());
		}
		else
		{
			auto iter = std::filesystem::directory_iterator(
				_folder,
				std::filesystem::directory_options::skip_permission_denied,
				ec);
			if (ec)
				return;
			iterate(iter, std::filesystem::directory_iterator());
		}
	}

}

#endif // FILE_SYSTEM_INFO_H_
