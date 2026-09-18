/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "RecentFilesManager.h"
#include "SettingsManager.h"
#include "FileSystemInfo.h"
#include "MyGUI_FileSystemUtility.h"

namespace tools
{

	MYGUI_SINGLETON_DEFINITION(RecentFilesManager);

	RecentFilesManager::RecentFilesManager() :
		mSingletonHolder(this)
	{
	}

	void RecentFilesManager::initialise()
	{
		if (!SettingsManager::getInstance().tryGetValue<size_t>("Files/MaxRecentFolders", mMaxRecentFolders))
			mMaxRecentFolders = 8;

		if (!SettingsManager::getInstance().tryGetValue<size_t>("Files/MaxRecentFiles", mMaxRecentFiles))
			mMaxRecentFiles = 8;

		mRecentFolder = MyGUI::utility::toPath(SettingsManager::getInstance().getValue("Files/RecentFolder"));

		mRecentFolders = SettingsManager::getInstance().getValueList<MyGUI::UString>("Files/RecentFolder.List");

		mRecentFiles = SettingsManager::getInstance().getValueList<MyGUI::UString>("Files/RecentFile.List");

		checkArray(mRecentFolders, mMaxRecentFolders);
		checkArray(mRecentFiles, mMaxRecentFiles);
	}

	void RecentFilesManager::shutdown()
	{
		SettingsManager::getInstance().setValue("Files/RecentFolder", MyGUI::utility::toUtf8(mRecentFolder));

		SettingsManager::getInstance().setValueList("Files/RecentFolder.List", mRecentFolders);

		SettingsManager::getInstance().setValueList("Files/RecentFile.List", mRecentFiles);
	}

	void RecentFilesManager::addRecentFolder(const std::filesystem::path& _folder)
	{
		const auto folder = _folder.empty() ? std::filesystem::current_path() : _folder;

		mRecentFolders.insert(mRecentFolders.begin(), MyGUI::utility::toUtf8(folder));

		checkArray(mRecentFolders, mMaxRecentFolders);
	}

	const RecentFilesManager::VectorUString& RecentFilesManager::getRecentFolders() const
	{
		return mRecentFolders;
	}

	const std::filesystem::path& RecentFilesManager::getRecentFolder() const
	{
		return mRecentFolder;
	}

	void RecentFilesManager::setRecentFolder(const std::filesystem::path& _value)
	{
		mRecentFolder = _value;

		addRecentFolder(mRecentFolder);
	}

	void RecentFilesManager::checkArray(VectorUString& _array, size_t _maxElements)
	{
		for (size_t index = 0; index < _array.size(); ++index)
			_array.erase(std::remove(_array.begin() + index + 1, _array.end(), _array[index]), _array.end());

		while (_array.size() > _maxElements)
			_array.pop_back();
	}

	void RecentFilesManager::addRecentFile(const MyGUI::UString& _fileName)
	{
		mRecentFiles.insert(mRecentFiles.begin(), _fileName);

		checkArray(mRecentFiles, mMaxRecentFiles);
	}

	const RecentFilesManager::VectorUString& RecentFilesManager::getRecentFiles() const
	{
		return mRecentFiles;
	}

}
