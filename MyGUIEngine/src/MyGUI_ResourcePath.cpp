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

		void _MyGUIExport addResourceLocation(
			const Ogre::String& _name,
			const Ogre::String& _type,
			const Ogre::String& _group,
			bool _recursive,
			bool _subdirs)
		{
			if (_subdirs) {
				Ogre::Archive* pArch = Ogre::ArchiveManager::getSingleton().load( _name, _type );
				Ogre::StringVectorPtr vec = pArch->find("*", true, true);
				for (size_t pos=0; pos<vec->size(); ++pos) {
					std::string new_filename = _name + '/' + vec->at(pos);
					#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
						Ogre::ResourceGroupManager::getSingleton().removeResourceLocation(Ogre::String(macBundlePath() + "/" + new_filename), _group);
						Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Ogre::String(macBundlePath() + "/" + new_filename), _type, _group, _recursive);
					#else
						Ogre::ResourceGroupManager::getSingleton().removeResourceLocation(new_filename, _group);
						Ogre::ResourceGroupManager::getSingleton().addResourceLocation(new_filename, _type, _group, _recursive);
					#endif
				}
				vec.setNull();
				Ogre::ArchiveManager::getSingleton().unload(pArch);
			}
			#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
				Ogre::ResourceGroupManager::getSingleton().removeResourceLocation(Ogre::String(macBundlePath() + "/" + _name), _group);
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(Ogre::String(macBundlePath() + "/" + _name), _type, _group, _recursive);
			#else
				Ogre::ResourceGroupManager::getSingleton().removeResourceLocation(_name, _group);
				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(_name, _type, _group, _recursive);
			#endif
		}

		bool _MyGUIExport isFileExist(
			const Ogre::String& _filename,
			const Ogre::String& _group,
			bool _unique,
			bool _fullmatch)
		{
			size_t count = 0;
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, _filename);
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

		Ogre::String _MyGUIExport getResourcePath(
			const Ogre::String& _filename,
			const Ogre::String& _group,
			bool _fullpath,
			bool _unique,
			bool _fullmatch)
		{
			Ogre::String result;
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, _filename);

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

			return result;
		}

		VectorString _MyGUIExport getVectorResourcePath(
			const Ogre::String& _pattern,
			const Ogre::String& _group,
			bool _fullpath,
			bool _fullmatch)
		{
			VectorString result;
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(_group, _pattern);

			result.reserve(pFileInfo->size());

			for (Ogre::FileInfoList::iterator fi = pFileInfo->begin(); fi != pFileInfo->end(); ++fi ) {
				if (!_fullmatch || fi->path.empty()) {

					if (_fullpath) {
						std::string path = fi->archive->getName() + "/" + fi->filename;
						bool find = false;
						for (VectorString::iterator iter=result.begin(); iter!=result.end(); ++iter) {
							if (*iter == path) { find = true; break; }
						}
						if (!find) result.push_back(path);
					}
					else {
						bool find = false;
						for (VectorString::iterator iter=result.begin(); iter!=result.end(); ++iter) {
							if (*iter == fi->filename) { find = true; break; }
						}
						if (!find) result.push_back(fi->filename);
					}

				}
			}

			pFileInfo.setNull();

			return result;
		}
		
	} // namespace helper

} // namespace MyGUI
