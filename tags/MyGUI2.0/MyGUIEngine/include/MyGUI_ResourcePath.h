/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_RESOURCE_PATH_H__
#define __MYGUI_RESOURCE_PATH_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{
	namespace templates
	{
		template <class T>
		std::vector<Ogre::String> get_vector_resource_path(const Ogre::String & _mask, const Ogre::String & _group)
		{
			std::vector<Ogre::String> vec;
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, _mask);
			for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); fi++ ) {
				vec.push_back(fi->archive->getName() + "/" + fi->filename);
			}
			return vec;
		}

		template <class T>
		Ogre::String get_resource_path(const Ogre::String & _mask, const Ogre::String & _group)
		{
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, _mask);
			if (pFileInfo->size() != 1) return "";
			Ogre::String retval = pFileInfo->front().archive->getName() + "/" + pFileInfo->front().filename;
			pFileInfo.setNull();
			return retval;
		}

	} // namespace templates

	namespace helper
	{
		// возвращает вектор путей ресурсов по маске
		inline std::vector<Ogre::String> getVectorResourcePath(const Ogre::String & _mask, const Ogre::String & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME) {return templates::get_vector_resource_path<void>(_mask, _group);}
		// возвращает путь ресурса по маске
		inline Ogre::String getResourcePath(const Ogre::String & _mask, const Ogre::String & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME) {return templates::get_resource_path<void>(_mask, _group);}
	} // namespace helper

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_PATH_H__
