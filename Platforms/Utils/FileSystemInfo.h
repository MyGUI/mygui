#ifndef FILE_SYSTEM_INFO_H_
#define FILE_SYSTEM_INFO_H_

#include <MyGUI.h>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>
#include <cwctype>

namespace common
{

	struct FileInfo
	{
		FileInfo(const std::wstring& _name, bool _folder) :
			name(_name),
			folder(_folder)
		{
		}
		std::wstring name;
		bool folder;
	};
	using VectorFileInfo = std::vector<FileInfo>;
	using VectorWString = std::vector<std::wstring>;

	inline bool matchWildcard(const std::wstring& _pattern, const std::wstring& _str)
	{
		size_t pi = 0, si = 0, starPi = std::wstring::npos, starSi = 0;
		while (si < _str.size())
		{
			if (pi < _pattern.size() && (_pattern[pi] == _str[si] || _pattern[pi] == L'?'))
			{
				++pi;
				++si;
			}
			else if (pi < _pattern.size() && _pattern[pi] == L'*')
			{
				starPi = pi++;
				starSi = si;
			}
			else if (starPi != std::wstring::npos)
			{
				pi = starPi + 1;
				si = ++starSi;
			}
			else
				return false;
		}
		while (pi < _pattern.size() && _pattern[pi] == L'*')
			++pi;
		return pi == _pattern.size();
	}

	inline std::wstring concatenatePath(const std::wstring& _base, const std::wstring& _name)
	{
		if (_base.empty())
			return _name;
		return (std::filesystem::path(_base) / _name).wstring();
	}

	inline std::wstring getSystemCurrentFolder()
	{
		return std::filesystem::current_path().wstring();
	}

	inline bool isParentDir(const std::wstring& _name)
	{
		return _name == L"..";
	}

	inline void getSystemFileList(
		VectorFileInfo& _result,
		const std::wstring& _folder,
		const std::wstring& _mask,
		bool _sorted = true)
	{
		std::error_code ec;

		const auto maskPath = std::filesystem::path(_mask);

		const auto searchDir = std::filesystem::path(_folder) / maskPath.parent_path();

		const auto pattern = maskPath.filename() == L"*.*" ? std::wstring{} : maskPath.filename().wstring();

		auto iter = std::filesystem::directory_iterator(
			searchDir,
			std::filesystem::directory_options::skip_permission_denied,
			ec);
		if (ec)
		{
			MYGUI_LOG(Error, "Can't open " + MyGUI::UString(searchDir.wstring()).asUTF8());
			return;
		}

		// ".." is not returned by directory_iterator; add it manually so file dialogs can navigate up
		_result.emplace_back(L"..", true);

		for (const auto& entry : iter)
		{
			std::wstring name = entry.path().filename().wstring();

			if (!pattern.empty() && !matchWildcard(pattern, name))
				continue;

			_result.emplace_back(name, entry.is_directory());
		}

		if (_sorted)
			std::sort(
				_result.begin(),
				_result.end(),
				[](const FileInfo& a, const FileInfo& b)
				{
					if (a.folder != b.folder)
						return a.folder;
					return std::lexicographical_compare(
						a.name.begin(),
						a.name.end(),
						b.name.begin(),
						b.name.end(),
						[](wchar_t c1, wchar_t c2) { return std::towlower(c1) < std::towlower(c2); });
				});
	}

	inline void scanFolder(
		VectorWString& _result,
		const std::wstring& _folder,
		bool _recursive,
		const std::wstring& _mask,
		bool _fullpath)
	{
		// Normalize DOS-style *.* (match all)
		std::wstring pattern = (_mask == L"*.*") ? std::wstring() : _mask;

		std::error_code ec;

		auto iterate = [&](auto iter, auto end) -> void
		{
			for (; iter != end; ++iter)
			{
				const auto& entry = *iter;
				std::wstring name = entry.path().filename().wstring();
				if (entry.is_directory())
					continue;

				if (!pattern.empty() && !matchWildcard(pattern, name))
					continue;

				if (_fullpath)
					_result.push_back(entry.path().wstring());
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
