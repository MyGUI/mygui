#ifndef __COMMON_H__
#define __COMMON_H__

inline const MyGUI::UString localise(const MyGUI::UString & _str)
{
	return MyGUI::LanguageManager::getInstance().getTag(_str);
}

#endif // __COMMON_H__
