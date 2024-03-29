/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "RecentFilesManager.h"
#include "SettingsManager.h"
#include "FileSystemInfo/FileSystemInfo.h"

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

		if (!SettingsManager::getInstance().tryGetValue<size_t>("Files/MaxRecentProjects", mMaxRecentProjects))
			mMaxRecentProjects = 8;

		mRecentFolder = SettingsManager::getInstance().getValue("Files/RecentFolder");
		mRecentFolders = SettingsManager::getInstance().getValueList<MyGUI::UString>("Files/RecentFolder.List");
		mRecentFiles = SettingsManager::getInstance().getValueList<MyGUI::UString>("Files/RecentFile.List");
		mRecentProjects = SettingsManager::getInstance().getValueList<MyGUI::UString>("Files/RecentProject.List");

		checkArray(mRecentFolders, mMaxRecentFolders);
		checkArray(mRecentFiles, mMaxRecentFiles);
		checkArray(mRecentProjects, mMaxRecentProjects);
	}

	void RecentFilesManager::shutdown()
	{
		SettingsManager::getInstance().setValue("Files/RecentFolder", mRecentFolder);
		SettingsManager::getInstance().setValueList<MyGUI::UString>("Files/RecentFolder.List", mRecentFolders);
		SettingsManager::getInstance().setValueList<MyGUI::UString>("Files/RecentFile.List", mRecentFiles);
		SettingsManager::getInstance().setValueList<MyGUI::UString>("Files/RecentProject.List", mRecentProjects);
	}

	void RecentFilesManager::addRecentFolder(const MyGUI::UString& _folder)
	{
		MyGUI::UString folder(_folder);
		if (_folder.empty())
			folder = MyGUI::UString(common::getSystemCurrentFolder());

		mRecentFolders.insert(mRecentFolders.begin(), folder);

		checkArray(mRecentFolders, mMaxRecentFolders);
	}

	const RecentFilesManager::VectorUString& RecentFilesManager::getRecentFolders() const
	{
		return mRecentFolders;
	}

	const MyGUI::UString& RecentFilesManager::getRecentFolder() const
	{
		return mRecentFolder;
	}

	void RecentFilesManager::setRecentFolder(const MyGUI::UString& _value)
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

		SettingsManager::getInstance().setValueList<MyGUI::UString>("Files/RecentFile.List", mRecentFiles);
	}

	const RecentFilesManager::VectorUString& RecentFilesManager::getRecentFiles() const
	{
		return mRecentFiles;
	}

	void RecentFilesManager::addRecentProject(const MyGUI::UString& _fileName)
	{
		mRecentProjects.insert(mRecentProjects.begin(), _fileName);

		checkArray(mRecentProjects, mMaxRecentProjects);

		SettingsManager::getInstance().setValueList<MyGUI::UString>("Files/RecentProject.List", mRecentProjects);
	}

	const RecentFilesManager::VectorUString& RecentFilesManager::getRecentProjects() const
	{
		return mRecentProjects;
	}

}
