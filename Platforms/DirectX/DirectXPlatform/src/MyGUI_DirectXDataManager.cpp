/*!
	@file
	@author		Losev Vasiliy aka bool
	@date		06/2009
*/

#include "MyGUI_DirectXDataManager.h"
#include "MyGUI_DirectXDiagnostic.h"
#include "MyGUI_DataFileStream.h"
#include "FileSystemInfo/FileSystemInfo.h"
#include <fstream>

namespace MyGUI
{

	DirectXDataManager::DirectXDataManager() :
		mIsInitialise(false)
	{
	}

	void DirectXDataManager::initialise()
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void DirectXDataManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IDataStream* DirectXDataManager::getData(const std::string& _name) const
	{
		std::string filepath = getDataPath(_name);
		if (filepath.empty())
			return nullptr;

		auto stream = std::make_unique<std::ifstream>();
		stream->open(filepath.c_str(), std::ios_base::binary);

		if (!stream->is_open())
			return nullptr;

		DataFileStream* data = new DataFileStream(std::move(stream));

		return data;
	}

	void DirectXDataManager::freeData(IDataStream* _data)
	{
		delete _data;
	}

	bool DirectXDataManager::isDataExist(const std::string& _name) const
	{
		const VectorString& files = getDataListNames(_name);
		return !files.empty();
	}

	const VectorString& DirectXDataManager::getDataListNames(const std::string& _pattern) const
	{
		static VectorString result;
		common::VectorWString wresult;
		result.clear();

		for (const auto& path : mPaths)
		{
			common::scanFolder(wresult, path.name, path.recursive, MyGUI::UString(_pattern).asWStr(), false);
		}

		for (const auto& file : wresult)
		{
			result.push_back(MyGUI::UString(file).asUTF8());
		}

		return result;
	}

	std::string DirectXDataManager::getDataPath(const std::string& _name) const
	{
		VectorString result;
		common::VectorWString wresult;

		for (const auto& path : mPaths)
		{
			common::scanFolder(wresult, path.name, path.recursive, MyGUI::UString(_name).asWStr(), true);
		}

		for (const auto& file : wresult)
		{
			result.push_back(MyGUI::UString(file).asUTF8());
		}

		if (!result.empty())
		{
			const std::string& path = result[0];
			if (result.size() > 1)
			{
				MYGUI_PLATFORM_LOG(
					Warning,
					"There are several files with name '" << _name << "'. '" << path << "' was used.");
				MYGUI_PLATFORM_LOG(Warning, "Other candidates are:");
				for (size_t index = 1; index < result.size(); index++)
					MYGUI_PLATFORM_LOG(Warning, " - '" << result[index] << "'");
			}
			return path;
		}

		return {};
	}

	void DirectXDataManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		ArhivInfo info;
		info.name = MyGUI::UString(_name).asWStr();
		info.recursive = _recursive;
		mPaths.push_back(info);
	}

} // namespace MyGUI
