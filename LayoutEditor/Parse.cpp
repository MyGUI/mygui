/*!
	@file
	@author		Georgiy Evmenov
	@date		10/2008
	@module
*/
#include "Parse.h"

namespace Parse
{
	bool checkParceFileName(MyGUI::EditPtr _edit)
	{
		static const Ogre::UTFString colour = "#FF0000";
		const Ogre::UTFString & text = _edit->getOnlyText();
		size_t index = _edit->getTextCursor();
		bool success = false;

		if (text.find_first_of("*?") == std::string::npos) {
			Ogre::FileInfoListPtr pFileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, text);
			// файл должен быть только один, если будет два, то все равно гуй его не съест
			if (pFileInfo->size() != 1) {
				_edit->setCaption(colour + text);
			}
			else {
				_edit->setCaption(text);
				success = true;
			}
			pFileInfo.setNull();
		}
		else {
			_edit->setCaption(colour + text);
		}
		_edit->setTextCursor(index);
		return success;
	}

	bool checkType(MyGUI::EditPtr _edit, std::string _type, bool _position_relative_mode)
	{
		bool success = true;
		//if ("Name" == _type) widget_for_type = 0;
		//else if ("Skin" == _type) widget_for_type = 1;
		//else
		if ("Position" == _type){
			if (_position_relative_mode)
				success = Parse::checkParce<float>(_edit, 4);
			else
				success = Parse::checkParce<int>(_edit, 4);
		}
		//else if ("Layer" == _type) // выбранное из комбы всегда корректно, а если в лейауте криво было - сами виноваты
		//else if ("String" == _type) // неправильная строка? O_o
		//else if ("Align" == _type) // выбранное из комбы всегда корректно, а если в лейауте криво было - сами виноваты
		else if ("1 int" == _type) success = Parse::checkParce<int>(_edit, 1);
		else if ("2 int" == _type) success = Parse::checkParce<int>(_edit, 2);
		else if ("4 int" == _type) success = Parse::checkParce<int>(_edit, 4);
		else if ("1 float" == _type) success = Parse::checkParce<float>(_edit, 1);
		else if ("2 float" == _type) success = Parse::checkParce<float>(_edit, 2);
		// надо сделать колорпикером и без проверки FIXME
		//else if ("Colour" == _type) success = Parse::checkParce<float>(_edit, 4);
		else if ("FileName" == _type) success = Parse::checkParceFileName(_edit);
		return success;
	}

}// namespace Parse
