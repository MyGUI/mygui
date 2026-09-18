/*!
	@file
	@author		George Evmenov
	@date		07/2009
*/

#include "MyGUI_OpenGLDataManager.h"
#include "MyGUI_OpenGLDiagnostic.h"
#include "MyGUI_DataFileStream.h"
#include "FileSystemInfo.h"
#include <fstream>
#include "MyGUI_FileSystemUtility.h"

namespace MyGUI
{

	void OpenGLDataManager::initialise()
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void OpenGLDataManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IDataStream* OpenGLDataManager::getData(const std::string& _name) const
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

	void OpenGLDataManager::freeData(IDataStream* _data)
	{
		delete _data;
	}

	bool OpenGLDataManager::isDataExist(const std::string& _name) const
	{
		const VectorString& files = getDataListNames(_name);
		return !files.empty();
	}

	VectorString OpenGLDataManager::getDataListNames(const std::string& _pattern) const
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

	std::string OpenGLDataManager::getDataPath(const std::string& _name) const
	{
		return MyGUI::utility::pathToUTF8(findDataPath(_name));
	}

	std::filesystem::path OpenGLDataManager::findDataPath(const std::string& _name) const
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

	void OpenGLDataManager::addResourceLocation(const std::filesystem::path& _name, bool _recursive)
	{
		mPaths.push_back({_name, _recursive});
	}

} // namespace MyGUI
