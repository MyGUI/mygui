/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#include "MyGUI_OsgDataManager.h"
#include "MyGUI_OsgDiagnostic.h"
#include "MyGUI_DataFileStream.h"
#include "FileSystemInfo.h"
#include <fstream>
#include "MyGUI_FileSystemUtility.h"

namespace MyGUI
{

	IDataStream* OsgDataManager::getData(const std::string& _name) const
	{
		const auto filepath = findDataPath(_name);
		if (filepath.empty())
			return nullptr;

		auto stream = std::make_unique<std::ifstream>();
		stream->open(filepath, std::ios_base::binary);

		if (!stream->is_open())
			return nullptr;

		DataFileStream* data = new DataFileStream(std::move(stream));

		return data;
	}

	void OsgDataManager::freeData(IDataStream* _data)
	{
		delete _data;
	}

	bool OsgDataManager::isDataExist(const std::string& _name) const
	{
		const VectorString& files = getDataListNames(_name);
		return !files.empty();
	}

	VectorString OsgDataManager::getDataListNames(const std::string& _pattern) const
	{
		VectorString result;
		common::VectorPath paths;

		for (const auto& path : mPaths)
		{
			common::scanFolder(paths, path.name, path.recursive, MyGUI::UString(_pattern), false);
		}

		for (const auto& file : paths)
		{
			result.push_back(MyGUI::utility::toUtf8(file));
		}

		return result;
	}

	std::string OsgDataManager::getDataPath(const std::string& _name) const
	{
		return MyGUI::utility::toUtf8(findDataPath(_name));
	}

	std::filesystem::path OsgDataManager::findDataPath(const std::string& _name) const
	{
		common::VectorPath paths;

		for (const auto& path : mPaths)
		{
			common::scanFolder(paths, path.name, path.recursive, MyGUI::UString(_name), true);
		}

		if (!paths.empty())
		{
			const auto& path = paths[0];
			if (paths.size() > 1)
			{
				MYGUI_PLATFORM_LOG(
					Warning,
					"There are several files with name '"
						<< _name << "'. '" << MyGUI::utility::toUtf8(path) << "' was used.");
				MYGUI_PLATFORM_LOG(Warning, "Other candidates are:");
				for (size_t index = 1; index < paths.size(); index++)
					MYGUI_PLATFORM_LOG(Warning, " - '" << MyGUI::utility::toUtf8(paths[index]) << "'");
			}
			return path;
		}

		return {};
	}

	void OsgDataManager::addResourceLocation(const std::filesystem::path& _name, bool _recursive)
	{
		mPaths.push_back({_name, _recursive});
	}

} // namespace MyGUI
