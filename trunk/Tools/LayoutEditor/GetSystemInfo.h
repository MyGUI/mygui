/*!
	@file
	@author		Albert Semenov
	@date		06/2009
	@module
*/
#ifndef __GET_SYSTEM_INFO_H__
#define __GET_SYSTEM_INFO_H__

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <string>
#include <vector>
#include <io.h>


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

	bool isAbsolutePath(const wchar_t* path)
	{
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		if (isalpha(path[0]) && path[1] == ':')
			return true;
	#endif
		return path[0] == '/' || path[0] == '\\';
	}

	std::wstring concatenatePath(const std::wstring& _base, const std::wstring& _name)
	{
		if (_base.empty() || isAbsolutePath(_name.c_str()))
			return _name;
		else
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
			return _base + L'\\' + _name;
#else
			return _base + L'/' + _name;
#endif
	}

	bool isReservedDir (const wchar_t *_fn)
	{
		// if "." /*or ".."*/
		return (_fn [0] == '.' && (_fn [1] == 0 /*|| (_fn [1] == '.' && _fn [2] == 0)*/));
	}

	void getSystemFileList(VectorFileInfo& _result, const std::wstring& _folder, const std::wstring& _mask)
	{
		//FIXME add optional parameter?
		bool ms_IgnoreHidden = true;

		long lHandle, res;
		struct _wfinddata_t tagData;

		// pattern can contain a directory name, separate it from mask
		size_t pos1 = _mask.rfind ('/');
		size_t pos2 = _mask.rfind ('\\');
		if (pos1 == _mask.npos || ((pos2 != _mask.npos) && (pos1 < pos2)))
			pos1 = pos2;
		std::wstring directory;
		if (pos1 != _mask.npos)
			directory = _mask.substr (0, pos1 + 1);

		std::wstring full_mask = concatenatePath(_folder, _mask);

		lHandle = _wfindfirst(full_mask.c_str(), &tagData);
		res = 0;
		while (lHandle != -1 && res != -1)
		{
			if (( !ms_IgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0 ) &&
				(!isReservedDir (tagData.name)))
			{
				_result.push_back(FileInfo(directory + tagData.name, _folder, (tagData.attrib & _A_SUBDIR) != 0));
			}
			res = _wfindnext( lHandle, &tagData );
		}
		// Close if we found any files
		if(lHandle != -1)
			_findclose(lHandle);
	}

	std::wstring getSystemCurrentFolder()
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		wchar_t buff[MAX_PATH+1];
		::GetCurrentDirectoryW(MAX_PATH, buff);
		return buff;
#else
		char buff[MAX_PATH+1];
		getcwd(buff, MAX_PATH);
		return MyGUI::UString(buff).as_WStr();
#endif
	}

}

#endif // __GET_SYSTEM_INFO_H__
