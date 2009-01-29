/*!
	@file
	@author		Georgiy Evmenov
	@date		10/2008
	@module
*/
#ifndef __PARSE_H__
#define __PARSE_H__

#include <MyGUI.h>

namespace Parse
{
	template <typename T>
	bool checkParce(MyGUI::EditPtr _edit, size_t _count)
	{
		static const Ogre::UTFString colour = MyGUI::LanguageManager::getInstance().getTag("ColourError");
		const Ogre::UTFString & text = _edit->getOnlyText();
		size_t index = _edit->getTextCursor();
		bool success = false;

		T p;
		std::istringstream str(text);
		while (_count > 0) {
			str >> p;
			-- _count;
		}
		if (str.fail()) {
			success = false;
		}
		else {
			std::string tmp;
			str >> tmp;
			if (!str.fail() || tmp.find_first_not_of(" \t\r") != std::string::npos) {
				success = false;
			}
			else {
				success = true;
			}
		}
		if (success) _edit->setCaption(text);
		else _edit->setCaption(colour + text);
		_edit->setTextCursor(index);
		return success;
	}

	bool checkParceFileName(MyGUI::EditPtr _edit);
}// namespace Parse
#endif // __PARSE_H__
