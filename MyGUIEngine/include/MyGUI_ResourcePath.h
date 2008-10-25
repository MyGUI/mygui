/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_RESOURCE_PATH_H__
#define __MYGUI_RESOURCE_PATH_H__

#include "MyGUI_Prerequest.h"

#include <OgrePrerequisites.h>
#include <OgreResourceGroupManager.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	namespace helper
	{
		typedef std::vector<Ogre::String> VectorString;

		void _MyGUIExport addResourceLocation(
			const Ogre::String& _name,
			const Ogre::String& _type,
			const Ogre::String& _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			bool _recursive = false,
			bool _subdirs = false);

		bool _MyGUIExport isFileExist(
			const Ogre::String& _filename,
			const Ogre::String& _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			bool _unique = true,
			bool _fullmatch = true);

		Ogre::String _MyGUIExport getResourcePath(
			const Ogre::String& _filename,
			const Ogre::String& _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			bool _fullpath = true,
			bool _unique = true,
			bool _fullmatch = true);

		VectorString _MyGUIExport getVectorResourcePath(
			const Ogre::String& _pattern,
			const Ogre::String& _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			bool _fullpath = true,
			bool _fullmatch = true);

	} // namespace helper

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_PATH_H__
