/*!
	@file
	@author		Albert Semenov
	@date		05/2008
*/

#include "MyGUI_OgreDataManager.h"
#include "MyGUI_OgreDiagnostic.h"
#include "MyGUI_OgreDataStream.h"

#include <Ogre.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	template <> const char* Singleton<OgreDataManager>::mClassTypeName("OgreDataManager");

	OgreDataManager::OgreDataManager() :
		mIsInitialise(false)
	{
	}

	void OgreDataManager::initialise(const std::string& _group)
	{
		MYGUI_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << getClassTypeName());

		mGroup = _group;

		MYGUI_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void OgreDataManager::shutdown()
	{
		MYGUI_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_LOG(Info, "* Shutdown: " << getClassTypeName());

		MYGUI_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IDataStream* OgreDataManager::getData(const std::string& _name)
	{
		try
		{
			Ogre::DataStreamPtr stream = Ogre::ResourceGroupManager::getSingleton().openResource(_name, mGroup, true);
			OgreDataStream* data = new OgreDataStream(stream);

			return data;
		}
		catch(Ogre::FileNotFoundException)
		{
		}

		return nullptr;
	}

	bool OgreDataManager::isDataExist(const std::string& _name)
	{
		const VectorString& files = getDataListNames(_name);
		return (files.size() == 1);
	}

	const VectorString& OgreDataManager::getDataListNames(const std::string& _pattern)
	{
		return getDataListNames(_pattern, false);
	}

	const VectorString& OgreDataManager::getDataListNames(const std::string& _pattern, bool _fullpath)
	{
		static VectorString result;
		result.clear();

		Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(mGroup, _pattern);

		result.reserve(pFileInfo->size());

		for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); ++fi )
		{
			if (fi->path.empty())
			{
				bool found = false;
				for (VectorString::iterator iter=result.begin(); iter!=result.end(); ++iter)
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

		return result;
	}

	const std::string& OgreDataManager::getDataPath(const std::string& _name)
	{
		static std::string result;

		const VectorString& files = getDataListNames(_name, true);
		result = files.size() == 1 ? files[0] : "";

		return result;
	}

} // namespace MyGUI
