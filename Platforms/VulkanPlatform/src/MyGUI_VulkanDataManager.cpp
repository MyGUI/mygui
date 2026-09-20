/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_VulkanDataManager.h"
#include "MyGUI_VulkanDiagnostic.h"
#include "MyGUI_DataFileStream.h"
#include "FileSystemInfo.h"
#include <fstream>
#include "MyGUI_FileSystemUtility.h"

namespace MyGUI
{

	IDataStream* VulkanDataManager::getData(const std::string& _name) const
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

	void VulkanDataManager::freeData(IDataStream* _data)
	{
		delete _data;
	}

	bool VulkanDataManager::isDataExist(const std::string& _name) const
	{
		const VectorString& files = getDataListNames(_name);
		return !files.empty();
	}

	VectorString VulkanDataManager::getDataListNames(const std::string& _pattern) const
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

	std::string VulkanDataManager::getDataPath(const std::string& _name) const
	{
		return MyGUI::utility::toUtf8(findDataPath(_name));
	}

	std::filesystem::path VulkanDataManager::findDataPath(const std::string& _name) const
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

	void VulkanDataManager::addResourceLocation(const std::filesystem::path& _name, bool _recursive)
	{
		mPaths.push_back({_name, _recursive});
	}

} // namespace MyGUI
