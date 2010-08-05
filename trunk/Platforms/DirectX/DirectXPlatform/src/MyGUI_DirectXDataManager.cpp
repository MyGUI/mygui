/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#include "MyGUI_DataFileStream.h"
#include "MyGUI_DirectXDataManager.h"
#include "MyGUI_DirectXDiagnostic.h"
#include <fstream>

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

			}
			while (FindNextFile(file, &FindData));
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

	DirectXDataManager::DirectXDataManager() :
		mIsInitialise(false)
	{
	}

	void DirectXDataManager::initialise()
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void DirectXDataManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IDataStream* DirectXDataManager::getData(const std::string& _name)
	{
		std::string filepath = getDataPath(_name);
		if (filepath.empty())
			return nullptr;

		std::ifstream* stream = new std::ifstream();
		stream->open(filepath.c_str(), std::ios_base::binary);

		if (!stream->is_open())
		{
			delete stream;
			return nullptr;
		}

		DataFileStream* data = new DataFileStream(stream);

		return data;
	}

	bool DirectXDataManager::isDataExist(const std::string& _name)
	{
		const VectorString& files = getDataListNames(_name);
		return (files.size() == 1);
	}

	const VectorString& DirectXDataManager::getDataListNames(const std::string& _pattern)
	{
		static VectorString result;
		result.clear();

		for (VectorArhivInfo::iterator item=mPaths.begin(); item!=mPaths.end(); ++item)
		{
			scanFolder(result, (*item).name, (*item).recursive, _pattern, false);
		}

		return result;
	}

	const std::string& DirectXDataManager::getDataPath(const std::string& _name)
	{
		static std::string path;
		VectorString result;

		for (VectorArhivInfo::iterator item=mPaths.begin(); item!=mPaths.end(); ++item)
		{
			scanFolder(result, (*item).name, (*item).recursive, _name, true);
		}

		path = result.size() == 1 ? result[0] : "";
		return path;
	}

	void DirectXDataManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		ArhivInfo info;
		info.name = _name;
		info.recursive = _recursive;
		mPaths.push_back(info);
	}

} // namespace MyGUI
