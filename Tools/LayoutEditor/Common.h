#ifndef __COMMON_H__
#define __COMMON_H__

#include <MyGUI.h>

inline const MyGUI::UString localise(const MyGUI::UString & _str)
{
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

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

} // namespace tools

#endif // __COMMON_H__
