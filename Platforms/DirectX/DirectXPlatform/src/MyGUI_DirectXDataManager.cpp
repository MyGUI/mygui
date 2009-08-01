/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_DataFileStream.h"
#include "MyGUI_DirectXDataManager.h"
#include "MyGUI_DirectXDiagnostic.h"

#include <windows.h>

namespace MyGUI
{

	void scanFolder(VectorString& _result, const std::string& _folder, bool _recursive, const std::string& _mask, bool _fullpath)
	{
		std::string folder = _folder;
		if (!folder.empty()) folder += "\\";
		std::vector<std::string> dir;

		WIN32_FIND_DATA FindData;
		HANDLE file = FindFirstFile((folder + _mask).c_str(), &FindData);
		if (file != INVALID_HANDLE_VALUE)
		{
			do
			{
				std::string name = FindData.cFileName;
				if ((name == ".") || (name == "..")) continue;

				// если скрытый то игнорируем
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) continue;

				// если это дирректория, то запускаем в ней поиск
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					dir.push_back(folder + name);
				}
				else
				{
					if (_fullpath)
						_result.push_back(folder + name);
					else
						_result.push_back(name);
				}

			} while (FindNextFile(file, &FindData));
		}
		FindClose(file);

		if (_recursive)
		{
			// теперь проходим подкаталоги
			for (std::vector<std::string>::iterator iter = dir.begin(); iter!=dir.end(); ++iter)
			{
				scanFolder(_result, *iter, _recursive, _mask, _fullpath);
			}
		}
	}

	MYGUI_INSTANCE_IMPLEMENT(DirectXDataManager);

	void DirectXDataManager::initialise()
	{
		MYGUI_PLATFORM_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void DirectXDataManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		MYGUI_PLATFORM_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	IDataStream* DirectXDataManager::getData(const std::string& _name)
	{
		std::string file = getDataPath(_name, true, true, true);

		std::ifstream stream;
		stream.open(file.c_str());

		if (!stream.is_open())
		{
			return nullptr;
		}

		DataFileStream* data = new DataFileStream(&stream);

		return data;
	}

	bool DirectXDataManager::isDataExist(
		const std::string& _pattern,
		bool _unique,
		bool _fullmatch)
	{
		const VectorString& files = getVectorDataPath(_pattern, false, _fullmatch);
		if ((_unique && files.size() == 1) || !files.empty()) return true;
		return false;
	}

	std::string DirectXDataManager::getDataPath(
		const std::string& _pattern,
		bool _fullpath,
		bool _unique,
		bool _fullmatch)
	{
		const VectorString& files = getVectorDataPath(_pattern, _fullpath, _fullmatch);
		if ((_unique && files.size() == 1) || !files.empty()) return files[0];
		return "";
	}

	const VectorString& DirectXDataManager::getVectorDataPath(
		const std::string& _pattern,
		bool _fullpath,
		bool _fullmatch)
	{
		static VectorString result;
		result.clear();

		for (VectorArhivInfo::iterator item=mPaths.begin(); item!=mPaths.end(); ++item)
		{
			scanFolder(result, (*item).name, (*item).recursive, _pattern, _fullpath);
		}

		return result;
	}

	void DirectXDataManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		ArhivInfo info;
		info.name = _name;
		info.recursive = _recursive;
		mPaths.push_back(info);
	}

} // namespace MyGUI
