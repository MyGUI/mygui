/*!
	@file
	@author		Albert Semenov
	@date		05/2008
*/

#include "MyGUI_OgreNextDataManager.h"
#include "MyGUI_OgreNextDiagnostic.h"
#include "MyGUI_OgreNextDataStream.h"

#include <Ogre.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	void OgreNextDataManager::initialise(const std::string& _group)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mGroup = _group;
		if (mGroup == Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME)
			mAllGroups = true;
		else
			mAllGroups = false;

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void OgreNextDataManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IDataStream* OgreNextDataManager::getData(const std::string& _name) const
	{
		try
		{
			Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(_name, mGroup);
			OgreNextDataStream* data = new OgreNextDataStream(stream);

			return data;
		}
		catch (const Ogre::FileNotFoundException& _e)
		{
			MYGUI_PLATFORM_LOG(Warning, _e.getDescription());
		}

		return nullptr;
	}

	void OgreNextDataManager::freeData(IDataStream* _data)
	{
		delete _data;
	}

	bool OgreNextDataManager::isDataExist(const std::string& _name) const
	{
		if (mAllGroups)
			return Ogre::ResourceGroupManager::getSingleton().resourceExistsInAnyGroup(_name);
		else
			return Ogre::ResourceGroupManager::getSingleton().resourceExists(mGroup, _name);
	}

	VectorString OgreNextDataManager::getDataListNames(const std::string& _pattern) const
	{
		return getDataListNames(_pattern, false);
	}

	VectorString OgreNextDataManager::getDataListNames(const std::string& _pattern, bool _fullpath) const
	{
		VectorString result;

		VectorString search;
		if (mAllGroups)
		{
			Ogre::StringVector sp = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
			search.reserve(sp.size());
			for (const auto& i : sp)
				search.push_back(i);
		}
		else
			search.push_back(mGroup);

		std::vector<Ogre::FileInfoListPtr> pFileInfos;

		int resultSize = 0;
		for (const auto& i : search)
		{
			Ogre::FileInfoListPtr pFileInfo =
				Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(i, _pattern);
			resultSize += pFileInfo->size();
			if (!pFileInfo->empty())
				pFileInfos.push_back(pFileInfo);
			else
				pFileInfo.reset();
		}

		result.reserve(resultSize);

		for (auto pFileInfo : pFileInfos)
		{
			for (auto& fi : *pFileInfo)
			{
				if (fi.path.empty())
				{
					bool found = false;
					for (auto& iter : result)
					{
						if (iter == fi.filename)
						{
							found = true;
							break;
						}
					}
					if (!found)
					{
						result.push_back(_fullpath ? fi.archive->getName() + "/" + fi.filename : fi.filename);
					}
				}
			}

			pFileInfo.reset();
		}

		return result;
	}

	std::string OgreNextDataManager::getDataPath(const std::string& _name) const
	{
		const VectorString& files = getDataListNames(_name, true);
		if (!files.empty())
		{
			const std::string& result = files[0];
			if (files.size() > 1)
			{
				MYGUI_PLATFORM_LOG(
					Warning,
					"There are several files with name '" << _name << "'. '" << result << "' was used.");
				MYGUI_PLATFORM_LOG(Warning, "Other candidates are:");
				for (size_t index = 1; index < files.size(); index++)
					MYGUI_PLATFORM_LOG(Warning, " - '" << files[index] << "'");
			}
			return result;
		}

		return {};
	}

	void OgreNextDataManager::addResourceLocation(const std::string& _name, bool _recursive)
	{
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_name, "FileSystem", mGroup, _recursive);
	}

} // namespace MyGUI
