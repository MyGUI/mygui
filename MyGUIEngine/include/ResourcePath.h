#ifndef _RESOURCEPATH_H_
#define _RESOURCEPATH_H_

#include "Prerequest.h"
#include <vector>

namespace MyGUI
{
	namespace templates
	{
		template <class T>
		std::vector<Ogre::String> get_vector_resource_path(const Ogre::String & _mask)
		{
			std::vector<Ogre::String> vec;
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, _mask);
			for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); fi++ ) {
				vec.push_back(fi->archive->getName() + "\\" + fi->filename);
			}
			return vec;
		}

		template <class T>
		Ogre::String get_resource_path(const Ogre::String & _mask)
		{
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, _mask);
			if (pFileInfo->size() != 1) return "";
			return pFileInfo->front().archive->getName() + "\\" + pFileInfo->front().filename;
		}

	} // namespace templates

	namespace helper
	{
		// возвращает вектор путей ресурсов по маске
		inline std::vector<Ogre::String> getVectorResourcePath(const Ogre::String & _mask) {return templates::get_vector_resource_path<void>(_mask);}
		// возвращает путь ресурса по маске
		inline Ogre::String getResourcePath(const Ogre::String & _mask) {return templates::get_resource_path<void>(_mask);}
	} // namespace helper

} // namespace MyGUI

#endif // _RESOURCEPATH_H_