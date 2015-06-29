/*!
	@file
	@author		Albert Semenov
	@date		05/2008
*/

#include "MyGUI_Ogre2DataManager.h"
#include "MyGUI_Ogre2Diagnostic.h"
#include "MyGUI_Ogre2DataStream.h"

#include <Ogre.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	Ogre2DataManager::Ogre2DataManager() :
		mAllGroups(false),
		mIsInitialise(false)
	{
	}

	void Ogre2DataManager::initialise(const std::string& _group)
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

	void Ogre2DataManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IDataStream* Ogre2DataManager::getData(const std::string& _name)
	{
		try
		{
			Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(_name, mGroup, true);
			Ogre2DataStream* data = new Ogre2DataStream(stream);

			return data;
		}
		catch (const Ogre::FileNotFoundException& _e)
		{
			MYGUI_PLATFORM_LOG(Warning, _e.getDescription());
		}

		return nullptr;
	}

	void Ogre2DataManager::freeData(IDataStream* _data)
	{
		delete _data;
	}

	bool Ogre2DataManager::isDataExist(const std::string& _name)
	{
		if (mAllGroups)
			return Ogre::ResourceGroupManager::getSingleton().resourceExistsInAnyGroup(_name);
		else
			return Ogre::ResourceGroupManager::getSingleton().resourceExists(mGroup, _name);
	}

	const VectorString& Ogre2DataManager::getDataListNames(const std::string& _pattern)
	{
		return getDataListNames(_pattern, false);
	}

	const VectorString& Ogre2DataManager::getDataListNames(const std::string& _pattern, bool _fullpath)
	{
		static VectorString result;
		result.clear();

		VectorString search;
		if (mAllGroups)
		{
			Ogre::StringVector sp = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
			search.reserve(sp.size());
			for (size_t i = 0; i < sp.size(); i++)
				search.push_back(sp[i]);
		}
		else
			search.push_back(mGroup);

		std::vector<Ogre::FileInfoListPtr> pFileInfos;

		int resultSize = 0;
		for (size_t i = 0; i < search.size(); i++)
		{
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(search[i], _pattern);
			resultSize += pFileInfo->size();
			if (!pFileInfo->empty())
				pFileInfos.push_back(pFileInfo);
			else
				pFileInfo.setNull();
		}

		result.reserve(resultSize);

		for (size_t i = 0; i < pFileInfos.size(); i++)
		{
			Ogre::FileInfoListPtr pFileInfo = pFileInfos[i];
			for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); ++fi )
			{
				if (fi->path.empty())
				{
					bool found = false;
					for (VectorString::iterator iter = result.begin(); iter != result.end(); ++iter)
					{
						if (*iter == fi->filename)
						{
							found = true;
							break;
						}
					}
					if (!found)
					{
						result.push_back(_fullpath ? fi->archive->getName() + "/" + fi->filename : fi->filename);
					}
				}
			}

			pFileInfo.setNull();
		}

		return result;
	}

	const std::string& Ogre2DataManager::getDataPath(const std::string& _name)
	{
		static std::string result;

		const VectorString& files = getDataListNames(_name, true);
		if (!files.empty())
		{
			result = files[0];
			if (files.size() > 1)
			{
				MYGUI_PLATFORM_LOG(Warning, "There are several files with name '" << _name << "'. '" << result << "' was used.");
				MYGUI_PLATFORM_LOG(Warning, "Other candidater are:");
				for (size_t index = 1; index < files.size(); index ++)
					MYGUI_PLATFORM_LOG(Warning, " - '" << files[index] << "'");
			}
		}

		return result;
	}

} // namespace MyGUI
