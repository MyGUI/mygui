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
		void setCurrentLanguage(const std::string & _name);
		inline std::string getCurrentLanguage() { return mCurrentLanguage != mMapFile.end() ? mCurrentLanguage->first : ""; }

	private:
		void loadLanguage(const std::string & _file);


	private:
		MapString mMapFile;
		MapString::const_iterator mCurrentLanguage;

		MapString mMapLanguage;
	};

} // namespace MyGUI

#endif // __MYGUI_LANGUAGE_MANAGER_H__
