/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "RecentFilesManager.h"
#include "SettingsManager.h"
#include "FileSystemInfo/FileSystemInfo.h"

template <> tools::RecentFilesManager* MyGUI::Singleton<tools::RecentFilesManager>::msInstance = nullptr;
template <> const char* MyGUI::Singleton<tools::RecentFilesManager>::mClassTypeName("RecentFilesManager");

namespace tools
{
	RecentFilesManager::RecentFilesManager() :
		mMaxRecentFolders(8),
		mMaxRecentFiles(8)
	{
	}

	RecentFilesManager::~RecentFilesManager()
	{
	}

	void RecentFilesManager::initialise()
	{
		if (SettingsManager::getInstance().getSector("Settings")->getExistProperty("MaxRecentFolders"))
			mMaxRecentFolders = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<size_t>("MaxRecentFolders");
		if (SettingsManager::getInstance().getSector("Settings")->getExistProperty("MaxRecentFiles"))
			mMaxRecentFiles = SettingsManager::getInstance().getSector("Settings")->getPropertyValue<size_t>("MaxRecentFiles");
		mRecentFolder = SettingsManager::getInstance().getSector("Files")->getPropertyValue("RecentFolder");
		mRecentFolders = SettingsManager::getInstance().getSector("Files")->getPropertyValueList("RecentFolders");
		mRecentFiles = SettingsManager::getInstance().getSector("Files")->getPropertyValueList("RecentFiles");

		checkArray(mRecentFolders, mMaxRecentFolders);
		checkArray(mRecentFiles, mMaxRecentFiles);
	}

	void RecentFilesManager::shutdown()
	{
		SettingsManager::getInstance().getSector("Files")->setPropertyValue("RecentFolder", mRecentFolder);
		SettingsManager::getInstance().getSector("Files")->setPropertyValueList("RecentFolders", mRecentFolders);
		SettingsManager::getInstance().getSector("Files")->setPropertyValueList("RecentFiles", mRecentFiles);
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
		for (size_t index = 0; index < _array.size(); ++ index)
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

} // namespace tools
