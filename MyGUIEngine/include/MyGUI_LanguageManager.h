/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __MYGUI_LANGUAGE_MANAGER_H__
#define __MYGUI_LANGUAGE_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"

namespace MyGUI
{

	typedef std::map<std::string, std::string> MapString;
	typedef std::map<Ogre::UTFString, Ogre::UTFString> MapLanguageString;

	class _MyGUIExport LanguageManager
	{
		INSTANCE_HEADER(LanguageManager);

	public:
		void initialise();
		void shutdown();

	public:

		/** Load additional MyGUI *.language file */
		bool load(const std::string & _file, const std::string & _group = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		void _load(xml::xmlNodePtr _node, const std::string & _file);

		inline bool isLanguageExist(const std::string & _name) { return mMapFile.find(_name) != mMapFile.end(); }
		bool setCurrentLanguage(const std::string & _name);
		inline std::string getCurrentLanguage() { return mCurrentLanguage != mMapFile.end() ? mCurrentLanguage->first : ""; }

		/** Replace tags #{tagname} with appropriate string or keep #{tagname} in mo replacement found */
		Ogre::UTFString replaceTags(const Ogre::UTFString & _line);

		inline void addTag(const Ogre::UTFString & _tag, const Ogre::UTFString & _replace) { mMapLanguage[_tag] = _replace; }

	private:
		bool loadLanguage(const std::string & _file);
		void _loadLanguage(std::ifstream & _stream);
		void _loadLanguage(const Ogre::DataStreamPtr& stream);


	private:
		MapString mMapFile;
		MapString::const_iterator mCurrentLanguage;

		MapLanguageString mMapLanguage;
	};

} // namespace MyGUI

#endif // __MYGUI_LANGUAGE_MANAGER_H__
