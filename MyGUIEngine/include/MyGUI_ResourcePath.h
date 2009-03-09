/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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

		void MYGUI_EXPORT addResourceLocation(
			const Ogre::String& _name,
			const Ogre::String& _type,
			const Ogre::String& _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			bool _recursive = false,
			bool _subdirs = false);

		bool MYGUI_EXPORT isFileExist(
			const Ogre::String& _filename,
			const Ogre::String& _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			bool _unique = true,
			bool _fullmatch = true);

		Ogre::String MYGUI_EXPORT getResourcePath(
			const Ogre::String& _filename,
			const Ogre::String& _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			bool _fullpath = true,
			bool _unique = true,
			bool _fullmatch = true);

		VectorString MYGUI_EXPORT getVectorResourcePath(
			const Ogre::String& _pattern,
			const Ogre::String& _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			bool _fullpath = true,
			bool _fullmatch = true);

		bool MYGUI_EXPORT isResourceLocationExist(
			const Ogre::String& _name,
			const Ogre::String& _type,
			const Ogre::String& _group);

	} // namespace helper

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_PATH_H__
