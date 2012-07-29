/*!
	@file
	@author		Georgiy Evmenov
	@date		10/2008
*/

#include "Precompiled.h"
#include "Parse.h"

namespace tools
{
	namespace utility
	{

		bool checkParseFileName(MyGUI::EditBox* _edit)
		{
			const MyGUI::UString& text = _edit->getOnlyText();

			bool success = false;

			if (text.find_first_of("*?") == std::string::npos)
			{
				success = MyGUI::DataManager::getInstance().isDataExist(text);
			}
			else
			{
				success = false;
			}

			_setSuccessText(_edit, text, success);
			return success;
		}

		void _setSuccessText(MyGUI::EditBox* _edit, const MyGUI::UString& _text, bool _success)
		{
			size_t index = _edit->getTextCursor();
			MyGUI::UString text = MyGUI::TextIterator::toTagsString(_text);

			if (_success)
				_edit->setCaption(text);
			else
			{
				static const MyGUI::UString colour = MyGUI::LanguageManager::getInstance().getTag("ColourError");
				_edit->setCaption(colour + text);
			}

			_edit->setTextCursor(index);
		}

		bool _checkStreamFail(std::istringstream& str)
		{
			if (str.fail())
			{
				return false;
			}
			else
			{
				std::string tmp;
				str >> tmp;
				if (!str.fail() || tmp.find_first_not_of(" \t\r") != std::string::npos)
				{
					return false;
				}
				else
				{
					return true;
				}
			}
		}

	}
}
