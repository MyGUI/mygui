/*!
	@file
	@author		Georgiy Evmenov
	@date		10/2008
	@module
*/

#include "precompiled.h"
#include "Parse.h"

namespace Parse
{
	bool checkParceFileName(MyGUI::EditPtr _edit)
	{
		static const Ogre::UTFString colour = MyGUI::LanguageManager::getInstance().getTag("ColourError");
		const Ogre::UTFString & text = _edit->getOnlyText();
		size_t index = _edit->getTextCursor();
		bool success = false;

		if (text.find_first_of("*?") == std::string::npos) {
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, text);
			// файл должен быть только один, если будет два, то все равно гуй его не съест
			if (pFileInfo->size() != 1) {
				success = false;
			}
			else {
				success = true;
			}
			pFileInfo.setNull();
		}
		else {
			success = false;
		}
		if (success) _edit->setCaption(text);
		else _edit->setCaption(colour + text);
		_edit->setTextCursor(index);
		return success;
	}

}// namespace Parse
