/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/

#include "MyGUI_DirectX11DataManager.h"
#include "MyGUI_DirectX11Diagnostic.h"
#include "MyGUI_DataFileStream.h"
#include "FileSystemInfo.h"
#include <fstream>
#include "MyGUI_FileSystemUtility.h"

namespace MyGUI
{

	DirectX11DataManager::DirectX11DataManager() :
		mIsInitialise(false)
	{
	}

	void DirectX11DataManager::initialise()
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void DirectX11DataManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IDataStream* DirectX11DataManager::getData(const std::string& _name) const
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

	void DirectX11DataManager::freeData(IDataStream* _data)
	{
		delete _data;
	}

	bool DirectX11DataManager::isDataExist(const std::string& _name) const
	{
		const VectorString& files = getDataListNames(_name);
		return !files.empty();
	}

	VectorString DirectX11DataManager::getDataListNames(const std::string& _pattern) const
	{
		VectorString result;
		common::VectorPath paths;

		for (const auto& path : mPaths)
		{
			common::scanFolder(paths, path.name, path.recursive, MyGUI::UString(_pattern), false);
		}

		for (const auto& file : paths)
		{
			result.push_back(MyGUI::utility::pathToUTF8(file));
		}

		return result;
	}

	std::string DirectX11DataManager::getDataPath(const std::string& _name) const
	{
		return MyGUI::utility::pathToUTF8(findDataPath(_name));
	}

	std::filesystem::path DirectX11DataManager::findDataPath(const std::string& _name) const
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
						<< _name << "'. '" << MyGUI::utility::pathToUTF8(path) << "' was used.");
				MYGUI_PLATFORM_LOG(Warning, "Other candidates are:");
				for (size_t index = 1; index < paths.size(); index++)
					MYGUI_PLATFORM_LOG(Warning, " - '" << MyGUI::utility::pathToUTF8(paths[index]) << "'");
			}
			return path;
		}

		return {};
	}

	void DirectX11DataManager::addResourceLocation(const std::filesystem::path& _name, bool _recursive)
	{
		mPaths.push_back({_name, _recursive});
	}

} // namespace MyGUI
