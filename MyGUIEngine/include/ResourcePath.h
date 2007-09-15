#ifndef _RESOURCEPATH_H_
#define _RESOURCEPATH_H_

#include "Prerequest.h"
#include <vector>
#include <Ogre.h>

namespace MyGUI
{
	struct path
	{

		static void getResourcePath(std::vector<Ogre::String> & _vectorPath, const Ogre::String & _maskFileName)
		{
			_vectorPath.clear();
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, _maskFileName);
			for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); fi++ ) {
				_vectorPath.push_back(fi->archive->getName() + "\\" + fi->filename);
			}
		}

		static inline Ogre::String getFullPath(const Ogre::String & _fileName)
		{
			std::vector<Ogre::String> vectorPath;
			getResourcePath(vectorPath, _fileName);
			if (vectorPath.size() != 1) return "";
			return vectorPath[0];
		}

	};

} // namespace MyGUI

#endif // _RESOURCEPATH_H_