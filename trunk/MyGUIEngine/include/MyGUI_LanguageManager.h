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
#include "MyGUI_Types.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	class MYGUI_EXPORT LanguageManager :
		public Singleton<LanguageManager>,
		public MemberObsolete<LanguageManager>
	{
	public:
		LanguageManager();

		void initialise();
		void shutdown();

		/** Set current language for replacing #{} tags */
		void setCurrentLanguage(const std::string& _name);
		/** Get current language */
		const std::string& getCurrentLanguage() const;

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
		delegates::CMultiDelegate1<const std::string&>
			eventChangeLanguage;

		/** Event : Request tag.\n
			signature : void method(const MyGUI::UString& _tag, MyGUI::UString& _result);
			@param _tag Tag than should be translated.
			@param _result String that should be placed instead specified tag.
			@note If this event is empty and _tag not found - "#{_tag}" used by default.
		*/
		delegates::CDelegate2<const UString&, UString&>
			eventRequestTag;

	private:
		void _load(xml::ElementPtr _node, const std::string& _file, Version _version);

		bool loadLanguage(const std::string& _file, bool _user = false);
		void _loadLanguage(IDataStream* _stream, bool _user);
		void _loadLanguageXML(IDataStream* _stream, bool _user);

		UString replaceTagsPass(const UString& _line, bool& _replaceResult);

	private:
		typedef std::map<UString, UString> MapLanguageString;

		MapLanguageString mMapLanguage;
		MapLanguageString mUserMapLanguage;

		std::string mCurrentLanguageName;

		typedef std::map<std::string, VectorString> MapListString;
		MapListString mMapFile;

		bool mIsInitialise;
		std::string mXmlLanguageTagName;
	};

} // namespace MyGUI

#endif // __MYGUI_LANGUAGE_MANAGER_H__
