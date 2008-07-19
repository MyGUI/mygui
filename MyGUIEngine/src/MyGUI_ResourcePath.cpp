/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#include "MyGUI_ResourcePath.h"
#include "MyGUI_Common.h"

namespace MyGUI
{
	namespace helper
	{

		VectorString getVectorResourcePath(const Ogre::String & _mask, const Ogre::String & _group)
		{
			VectorString vec;
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, _mask);
			vec.reserve(pFileInfo->size());
			for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); ++fi ) {
				vec.push_back(fi->archive->getName() + "/" + fi->filename);
			}
			pFileInfo.setNull();
			return vec;
		}

		Ogre::String getResourcePath(const Ogre::String & _mask, const Ogre::String & _group)
		{
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, _mask);
			if (pFileInfo->size() > 1) {
				MYGUI_LOG(Error, "helper::getResourcePath : mask '" << _mask << "' a lot of concurrences.");
				for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); ++fi ) {
					MYGUI_LOG(Error, "helper::getResourcePath : found [" << (fi -  pFileInfo->begin()) << "] file : '" << fi->archive->getName() << "/" << fi->filename << "'");
				}
				return "";
			}
			else if (pFileInfo->size() < 1) {
				MYGUI_LOG(Error, "helper::getResourcePath : mask '" << _mask << "' not found.");
				return "";
			}
			Ogre::String retval = pFileInfo->front().archive->getName() + "/" + pFileInfo->front().filename;
			pFileInfo.setNull();
			return retval;
		}

	} // namespace helper
} // namespace MyGUI
