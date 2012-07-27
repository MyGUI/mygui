/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _f43354a9_ff2f_4a2e_b89a_c92dbcbdaf2c_
#define _f43354a9_ff2f_4a2e_b89a_c92dbcbdaf2c_

#include <MyGUI.h>

namespace tools
{

	inline MyGUI::UString replaceTags(const MyGUI::UString& _text)
	{
		return MyGUI::LanguageManager::getInstance().replaceTags(MyGUI::utility::toString("#{", _text, "}"));
	}

	inline void addUserTag(const MyGUI::UString& _tagText, const MyGUI::UString& _replaceText)
	{
		MyGUI::LanguageManager::getInstance().addUserTag(_tagText, _replaceText);
	}

}

#endif
