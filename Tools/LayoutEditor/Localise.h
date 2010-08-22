#ifndef __LOCALISE_H__
#define __LOCALISE_H__

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

#endif // __LOCALISE_H__
