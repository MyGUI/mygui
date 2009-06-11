/*!
	@file
	@author		Albert Semenov
	@date		06/2009
	@module
*/
#ifndef __GET_SYSTEM_INFO_W32_H__
#define __GET_SYSTEM_INFO_W32_H__

#include <windows.h>
#include <string>
#include <vector>

namespace common
{

	struct FileInfo
	{
		FileInfo(const std::wstring& _name, const std::wstring& _path, bool _folder) : name(_name), path(_path), folder(_folder) { }
		std::wstring path;
		std::wstring name;
		bool folder;
	};
	typedef std::vector<FileInfo> VectorFileInfo;

	std::wstring getSystemCurrentFolder()
	{
		wchar_t buff[MAX_PATH+1];
		::GetCurrentDirectoryW(MAX_PATH, buff);

		return buff;
	}

	typedef std::vector<std::wstring> VectorString;

	void getSystemFileList(VectorFileInfo& _result, const std::wstring& _folder, const std::wstring& _mask)
	{
		std::wstring folder = _folder;
		if (!folder.empty()) folder += L"\\";

		WIN32_FIND_DATAW FindData;
		HANDLE file = FindFirstFileW((folder + _mask).c_str(), &FindData);
		if (file != INVALID_HANDLE_VALUE)
		{
			do
			{
				std::wstring name = FindData.cFileName;
				if (name == L".") continue;

				// если скрытый то игнорируем
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) continue;

				// если это дирректория, то запускаем в ней поиск
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					_result.push_back(FileInfo(name, folder, true));
				}
				else
				{
					_result.push_back(FileInfo(name, folder, false));
				}

			} while (FindNextFileW(file, &FindData));
		}
		FindClose(file);
	}

}

#endif // __GET_SYSTEM_INFO_W32_H__
