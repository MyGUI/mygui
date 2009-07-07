/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
/*
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

	class MYGUI_EXPORT LanguageManager
	{
		MYGUI_INSTANCE_HEADER(LanguageManager);

	public:
		void initialise();
		void shutdown();

		/** Load additional MyGUI *_language.xml file */
		bool load(const std::string& _file);

		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);

		/** Return true if language _name exist */
		bool isLanguageExist(const std::string& _name);
		/** Set current language for replacing #{} tags */
		bool setCurrentLanguage(const std::string& _name);
		/** Get current language */
		std::string getCurrentLanguage();

		/** Replace all tags #{tagname} in _line with appropriate string dependent
		on current language or keep #{tagname} if 'tagname' not found found */
		UString replaceTags(const UString& _line);

		/** Get tag value */
		UString getTag(const UString& _tag);

		/** Add user tag */
		void addUserTag(const UString& _tag, const UString& _replace);

		/** Delete all user tags */
		void clearUserTags();

		/** Event : Change current language.\n
			signature : void method(const std::string& _language);
			@param _language Current language.
		*/
		delegates::CMultiDelegate1<const std::string &> eventChangeLanguage;

	private:
		void loadLanguage(const VectorString& _list);
		bool loadLanguage(const std::string& _file, bool _user = false);
		void _loadLanguage(std::istream& _stream, bool _user);

	private:
		typedef std::map<std::string, VectorString> MapListString;
		typedef std::map<std::string, std::string> MapString;
		typedef std::map<UString, UString> MapLanguageString;

		MapListString mMapFile;
		MapListString::const_iterator mCurrentLanguage;

		MapLanguageString mMapLanguage;
		MapLanguageString mUserMapLanguage;
	};

} // namespace MyGUI

#endif // __MYGUI_LANGUAGE_MANAGER_H__
