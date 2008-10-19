/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_RESOURCE_PATH_H__
#define __MYGUI_RESOURCE_PATH_H__

#include "MyGUI_Prerequest.h"

#include <Ogre.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	namespace helper
	{
		typedef std::vector<Ogre::String> VectorString;

		// возвращает вектор путей ресурсов по маске
		VectorString _MyGUIExport getVectorResourcePath(const Ogre::String & _mask, const Ogre::String & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		// возвращает путь ресурса по маске
		Ogre::String _MyGUIExport getResourcePath(const Ogre::String & _mask, const Ogre::String & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

	} // namespace helper

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_PATH_H__
