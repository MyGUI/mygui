/*!
	@file
	@author		Albert Semenov
	@date		09/2008
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
#ifndef __MYGUI_LANGUAGE_MANAGER_H__
#define __MYGUI_LANGUAGE_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{

	typedef std::vector<std::string> VectorString;
	typedef std::map<std::string, VectorString> MapListString;
	typedef std::map<std::string, std::string> MapString;
	typedef std::map<Ogre::UTFString, Ogre::UTFString> MapLanguageString;

	// делегат для смены оповещения смены языков
	typedef delegates::CMultiDelegate1<const std::string &> MultiDelegate_String;

	class MYGUI_EXPORT LanguageManager
	{
		MYGUI_INSTANCE_HEADER(LanguageManager);

	public:
		void initialise();
		void shutdown();

	public:

		/** Load additional MyGUI *_language.xml file */
		bool load(const std::string & _file, const std::string & _group = MyGUI::ResourceManager::GUIResourceGroupName);

		void _load(xml::ElementPtr _node, const std::string & _file, Version _version);

		/** Return true if language _name exist */
		bool isLanguageExist(const std::string & _name) { return mMapFile.find(_name) != mMapFile.end(); }
		/** Set current language for replacing #{} tags */
		bool setCurrentLanguage(const std::string & _name);
		/** Get current language */
		std::string getCurrentLanguage() { return mCurrentLanguage != mMapFile.end() ? mCurrentLanguage->first : ""; }

		/** Replace all tags #{tagname} in _line with appropriate string dependent
		on current language or keep #{tagname} if 'tagname' not found found */
		Ogre::UTFString replaceTags(const Ogre::UTFString & _line);

		/** Get tag value */
		Ogre::UTFString getTag(const Ogre::UTFString & _tag);

		/** Add user tag */
		void addUserTag(const Ogre::UTFString & _tag, const Ogre::UTFString & _replace) { mUserMapLanguage[_tag] = _replace; }

		/** Delete all user tags */
		void clearUserTags() { mUserMapLanguage.clear(); }

		/** Event : Change current language.\n
			signature : void method(const std::string & _language);
			@param _language Current language.
		*/
		MultiDelegate_String eventChangeLanguage;

	private:
		void loadLanguage(const VectorString & _list, const std::string & _group);
		bool loadLanguage(const std::string & _file, const std::string & _group, bool _user = false);
		void _loadLanguage(std::ifstream & _stream, bool _user);
		void _loadLanguage(const Ogre::DataStreamPtr& stream, bool _user);


	private:
		MapListString mMapFile;
		MapListString::const_iterator mCurrentLanguage;

		MapLanguageString mMapLanguage;
		MapLanguageString mUserMapLanguage;
	};

} // namespace MyGUI

#endif // __MYGUI_LANGUAGE_MANAGER_H__
