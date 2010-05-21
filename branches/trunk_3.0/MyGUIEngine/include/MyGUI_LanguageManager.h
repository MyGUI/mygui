/*!
	@file
	@author		Albert Semenov
	@date		09/2008
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
#include "MyGUI_Singleton.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_Delegate.h"

namespace MyGUI
{

	class MYGUI_EXPORT LanguageManager :
		public Singleton<LanguageManager>
	{
	public:
		LanguageManager();

		void initialise();
		void shutdown();

		/** Set current language for replacing #{} tags */
		void setCurrentLanguage(const std::string& _name);
		/** Get current language */
		const std::string& getCurrentLanguage();

		/** Replace all tags #{tagname} in _line with appropriate string dependent
		on current language or keep #{tagname} if 'tagname' not found found */
		UString replaceTags(const UString& _line);

		/** Get tag value */
		UString getTag(const UString& _tag);

		/** Add user tag */
		void addUserTag(const UString& _tag, const UString& _replace);

		/** Delete all user tags */
		void clearUserTags();

		/** Load user tags */
		bool loadUserTags(const std::string& _file);

		/** Event : Change current language.\n
			signature : void method(const std::string& _language);
			@param _language Current language.
		*/
		delegates::CMultiDelegate1<const std::string &> eventChangeLanguage;

	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : bool ResourceManager::load(const std::string& _file)")
		bool load(const std::string& _file);

#endif // MYGUI_DONT_USE_OBSOLETE

	private:
		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);

		bool loadLanguage(const std::string& _file, bool _user = false);
		void _loadLanguage(IDataStream* _stream, bool _user);
		void _loadLanguageXML(IDataStream* _stream, bool _user);

	private:
		typedef std::map<UString, UString> MapLanguageString;

		MapLanguageString mMapLanguage;
		MapLanguageString mUserMapLanguage;

		std::string mCurrentLanguageName;

		typedef std::vector<std::string> VectorString;
		typedef std::map<std::string, VectorString> MapListString;
		MapListString mMapFile;

		bool mIsInitialise;
	};

} // namespace MyGUI

#endif // __MYGUI_LANGUAGE_MANAGER_H__
