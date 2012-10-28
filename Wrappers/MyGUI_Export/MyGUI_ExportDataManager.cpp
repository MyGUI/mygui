/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#include "MyGUI_ExportDataManager.h"
#include "MyGUI_ExportDiagnostic.h"
#include "MyGUI_DataFileStream.h"
#include "FileSystemInfo/FileSystemInfo.h"

namespace MyGUI
{

	ExportDataManager& ExportDataManager::getInstance()
	{
		return *getInstancePtr();
	}

	ExportDataManager* ExportDataManager::getInstancePtr()
	{
		return static_cast<ExportDataManager*>(DataManager::getInstancePtr());
	}

	ExportDataManager::ExportDataManager()
	{
	}

	void ExportDataManager::initialise()
	{
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
	}

	void ExportDataManager::shutdown()
	{
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
	}

	IDataStream* ExportDataManager::getData(const std::string& _name)
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

	void ExportDataManager::freeData(IDataStream* _data)
	{
		delete _data;
	}

	bool ExportDataManager::isDataExist(const std::string& _name)
	{
		const VectorString& files = getDataListNames(_name);
		return !files.empty();
	}

	const VectorString& ExportDataManager::getDataListNames(const std::string& _pattern)
	{
		static VectorString result;
		common::VectorWString wresult;
		result.clear();

		for (VectorArhivInfo::const_iterator item = mPaths.begin(); item != mPaths.end(); ++item)
		{
			common::scanFolder(wresult, (*item).name, (*item).recursive, MyGUI::UString(_pattern).asWStr(), false);
		}

		for (common::VectorWString::const_iterator item = wresult.begin(); item != wresult.end(); ++item)
		{
			result.push_back(MyGUI::UString(*item).asUTF8());
		}

		return result;
	}

	const std::string& ExportDataManager::getDataPath(const std::string& _name)
	{
		static std::string path;
		VectorString result;
		common::VectorWString wresult;

		for (VectorArhivInfo::const_iterator item = mPaths.begin(); item != mPaths.end(); ++item)
		{
			common::scanFolder(wresult, (*item).name, (*item).recursive, MyGUI::UString(_name).asWStr(), true);
		}

		for (common::VectorWString::const_iterator item = wresult.begin(); item != wresult.end(); ++item)
		{
			result.push_back(MyGUI::UString(*item).asUTF8());
		}

		if (!result.empty())
		{
			path = result[0];
			if (result.size() > 1)
			{
				MYGUI_PLATFORM_LOG(Warning, "There are several files with name '" << _name << "'. '" << path << "' was used.");
				MYGUI_PLATFORM_LOG(Warning, "Other candidater are:");
				for (size_t index = 1; index < result.size(); index ++)
					MYGUI_PLATFORM_LOG(Warning, " - '" << result[index] << "'");
			}
		}

		return path;
	}

	void ExportDataManager::addResourceLocation(const std::string& _path, bool _recursive)
	{
		ArhivInfo info;
		info.name = MyGUI::UString(_path).asWStr();
		info.recursive = _recursive;
		mPaths.push_back(info);
	}

}
