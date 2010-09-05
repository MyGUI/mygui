/*!
	@file
	@author		Georgiy Evmenov
	@date		10/2008
*/
#ifndef __PARSE_H__
#define __PARSE_H__

#include <MyGUI.h>

namespace tools
{
	namespace utility
	{
		template <typename T>
		bool checkParseInterval(MyGUI::Edit* _edit, size_t _count, T _min, T _max)
		{
			static const MyGUI::UString colour = MyGUI::LanguageManager::getInstance().getTag("ColourError");
			const MyGUI::UString& text = _edit->getOnlyText();
			size_t index = _edit->getTextCursor();
			bool success = true;

			T p;
			std::istringstream str(text);
			while (success && _count > 0)
			{
				str >> p;
				if (p > _max || p < _min) success = false;
				-- _count;
			}
			if (success)
			{
				if (str.fail())
				{
					success = false;
				}
				else
				{
					std::string tmp;
					str >> tmp;
					if (!str.fail() || tmp.find_first_not_of(" \t\r") != std::string::npos)
					{
						success = false;
					}
					else
					{
						success = true;
					}
				}
			}

			if (success) _edit->setCaption(text);
			else _edit->setCaption(colour + text);
			_edit->setTextCursor(index);
			return success;
		}

		template <typename T>
		bool checkParse(MyGUI::Edit* _edit, size_t _count)
		{
			static const MyGUI::UString colour = MyGUI::LanguageManager::getInstance().getTag("ColourError");
			const MyGUI::UString& text = _edit->getOnlyText();
			size_t index = _edit->getTextCursor();
			bool success = false;

			T p;
			std::istringstream str(text);
			while (_count > 0)
			{
				str >> p;
				-- _count;
			}
			if (str.fail())
			{
				success = false;
			}
			else
			{
				std::string tmp;
				str >> tmp;
				if (!str.fail() || tmp.find_first_not_of(" \t\r") != std::string::npos)
				{
					success = false;
				}
				else
				{
					success = true;
				}
			}
			if (success) _edit->setCaption(text);
			else _edit->setCaption(colour + text);
			_edit->setTextCursor(index);
			return success;
		}

		bool checkParseFileName(MyGUI::Edit* _edit);

	} // namespace utility
} // namespace tools

#endif // __PARSE_H__
