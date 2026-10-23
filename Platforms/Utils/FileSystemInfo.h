#ifndef FILE_SYSTEM_INFO_H_
#define FILE_SYSTEM_INFO_H_

#include <MyGUI.h>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
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

	inline VectorFileInfo getSystemFileList(const std::filesystem::path& _folder)
	{
		std::error_code ec;
		auto iter = std::filesystem::directory_iterator(
			_folder,
			std::filesystem::directory_options::skip_permission_denied,
			ec);
		if (ec)
		{
			MYGUI_LOG(Error, "Can't open " + MyGUI::utility::toUtf8(_folder));
			return {};
		}

		VectorFileInfo result;
		// ".." is not returned by directory_iterator; add it manually so file dialogs can navigate up
		result.emplace_back("..", true);
		for (const auto& entry : iter)
			result.emplace_back(entry.path().filename(), entry.is_directory());

		std::sort(
			result.begin(),
			result.end(),
			[](const FileInfo& a, const FileInfo& b)
			{
				if (a.folder != b.folder)
					return a.folder;
				return a.name < b.name;
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
		std::error_code ec;

		auto iterate = [&](auto iter, auto end) -> void
		{
			for (; iter != end; ++iter)
			{
				const auto& entry = *iter;
				auto name = entry.path().filename();
				if (entry.is_directory())
					continue;

				if (!_mask.empty() && !matchWildcard(_mask, MyGUI::UString(MyGUI::utility::toUtf8(name))))
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
