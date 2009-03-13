/*!
	@file
	@author		Albert Semenov
	@date		07/2008
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

#include "MyGUI_Precompiled.h"
#include "MyGUI_ResourcePath.h"
#include "MyGUI_Common.h"
#include "MyGUI_Convert.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#include <CoreFoundation/CoreFoundation.h>
#endif

namespace MyGUI
{
	namespace helper
	{
		#if MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
		// This function will locate the path to our application on OS X,
		// unlike windows you can not rely on the curent working directory
		// for locating your configuration files and resources.
		std::string MYGUI_EXPORT macBundlePath()
		{
			char path[1024];
			CFBundleRef mainBundle = CFBundleGetMainBundle();    assert(mainBundle);
			CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);    assert(mainBundleURL);
			CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);    assert(cfStringRef);
			CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);
			CFRelease(mainBundleURL);
			CFRelease(cfStringRef);
			return std::string(path);
		}
		#endif

		void MYGUI_EXPORT addResourceLocation(
			const Ogre::String& _name,
			const Ogre::String& _type,
			const Ogre::String& _group,
			bool _recursive,
			bool _subdirs)
		{
			if (_subdirs) {
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
				Ogre::Archive* pArch = Ogre::ArchiveManager::getSingleton().load( convert::utf8_to_ansi(_name), _type );
#else
				Ogre::Archive* pArch = Ogre::ArchiveManager::getSingleton().load( _name, _type );
#endif
				Ogre::StringVectorPtr vec = pArch->find("*", true, true);
				for (size_t pos=0; pos<vec->size(); ++pos) {
					std::string new_filename = _name + '/' + vec->at(pos);
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
					if ( ! isResourceLocationExist(Ogre::String(macBundlePath() + "/" + new_filename), _type, _group) )
						Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Ogre::String(macBundlePath() + "/" + new_filename), _type, _group, _recursive);
#else
					if ( ! isResourceLocationExist(new_filename, _type, _group) )
						Ogre::ResourceGroupManager::getSingleton().addResourceLocation(new_filename, _type, _group, _recursive);
#endif
				}
				vec.setNull();
				Ogre::ArchiveManager::getSingleton().unload(pArch);
			}
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
			if ( ! isResourceLocationExist(Ogre::String(macBundlePath() + "/" + _name), _type, _group) )
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Ogre::String(macBundlePath() + "/" + _name), _type, _group, _recursive);
#else
			if ( ! isResourceLocationExist(_name, _type, _group) )
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_name, _type, _group, _recursive);
#endif
		}

		bool MYGUI_EXPORT isFileExist(
			const Ogre::String& _filename,
			const Ogre::String& _group,
			bool _unique,
			bool _fullmatch)
		{
			size_t count = 0;
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, convert::utf8_to_ansi(_filename));
#else
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, _filename);
#endif
			if (_fullmatch) {
				for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); ++fi ) {
					if (fi->path.empty()) count ++;
				}
			}
			else {
				count = pFileInfo->size();
			}

			pFileInfo.setNull();

			// лог для мультиресульта
			if (_unique && count > 1) { getResourcePath(_filename, _group); }

			return _unique ? count == 1  : count > 0;
		}

		Ogre::String MYGUI_EXPORT getResourcePath(
			const Ogre::String& _filename,
			const Ogre::String& _group,
			bool _fullpath,
			bool _unique,
			bool _fullmatch)
		{
			Ogre::String result;
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, convert::utf8_to_ansi(_filename));
#else
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, _filename);
#endif

			// подчищаем те что не точно соответствуют
			if (_fullmatch) {
				for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end();  ) {
					if (fi->path.empty()) ++fi;
					else fi = pFileInfo->erase(fi);
				}
			}

			// только уникальный
			if (_unique) {
				if (pFileInfo->size() > 1) {
					MYGUI_LOG(Error, "helper::getFilePath : filename '" << _filename << "' a lot of concurrences.");
					for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); ++fi ) {
						MYGUI_LOG(Error, "  * helper::getFilePath : found [" << (fi -  pFileInfo->begin()) << "] file : '" << fi->archive->getName() << "/" << fi->filename << "'");
					}
					return result;
				}
				else if (pFileInfo->size() == 0) {
					MYGUI_LOG(Error, "helper::getFilePath : filename '" << _filename << "' not found.");
					return result;
				}
			}
			// первый попавшийся
			else {
				if (pFileInfo->size() == 0) {
					MYGUI_LOG(Error, "helper::getFilePath : filename '" << _filename << "' not found.");
					return result;
				}
			}

			// полный путь или нет
			result = _fullpath ? pFileInfo->front().archive->getName() + "/" + pFileInfo->front().filename : pFileInfo->front().filename;

			pFileInfo.setNull();

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			result = convert::ansi_to_utf8(result);
#endif

			return result;
		}

		VectorString MYGUI_EXPORT getVectorResourcePath(
			const Ogre::String& _pattern,
			const Ogre::String& _group,
			bool _fullpath,
			bool _fullmatch)
		{
			VectorString result;
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, convert::utf8_to_ansi(_pattern));
#else
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, _pattern);
#endif

			result.reserve(pFileInfo->size());

			for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); ++fi ) {
				if (!_fullmatch || fi->path.empty()) {

					if (_fullpath) {
						std::string path = fi->archive->getName() + "/" + fi->filename;
						bool find = false;
						for (VectorString::iterator iter=result.begin(); iter!=result.end(); ++iter) {
							if (*iter == path) { find = true; break; }
						}
						if (!find) {
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
							result.push_back(convert::ansi_to_utf8(path));
#else
							result.push_back(path);
#endif
						}
					}
					else {
						bool find = false;
						for (VectorString::iterator iter=result.begin(); iter!=result.end(); ++iter) {
							if (*iter == fi->filename) { find = true; break; }
						}
						if (!find) {
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
							result.push_back(convert::ansi_to_utf8(fi->filename));
#else
							result.push_back(fi->filename);
#endif
						}
					}

				}
			}

			pFileInfo.setNull();

			return result;
		}

		bool isResourceLocationExist(
			const Ogre::String& _name,
			const Ogre::String& _type,
			const Ogre::String& _group)
		{
			Ogre::FileInfoListPtr list_info = Ogre::ResourceGroupManager::getSingleton().listResourceFileInfo(_group);
			for (Ogre::FileInfoList::iterator fi = list_info->begin(); fi != list_info->end(); ++fi )
			{
				if (fi->archive->getName() == _name && fi->archive->getType() == _type) return true;
			}
			return false;
		}

	} // namespace helper

} // namespace MyGUI
