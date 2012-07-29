/*!
	@file
	@author		Georgiy Evmenov
	@date		10/2008
*/

#ifndef _d5b3c9c5_7ae6_45c9_aaf6_fd784ef1ad0e_
#define _d5b3c9c5_7ae6_45c9_aaf6_fd784ef1ad0e_

#include <MyGUI.h>

namespace tools
{
	namespace utility
	{

		void _setSuccessText(MyGUI::EditBox* _edit, const MyGUI::UString& _text, bool _success);
		bool _checkStreamFail(std::istringstream& str);

		template <typename T>
		bool checkParseInterval(MyGUI::EditBox* _edit, size_t _count, T _min, T _max)
		{
			const MyGUI::UString& text = _edit->getOnlyText();
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
				success = _checkStreamFail(str);
			}

			_setSuccessText(_edit, text, success);
			return success;
		}

		template <typename T>
		bool checkParse(MyGUI::EditBox* _edit, size_t _count)
		{
			const MyGUI::UString& text = _edit->getOnlyText();
			bool success = false;

			T p;
			std::istringstream str(text);
			while (_count > 0)
			{
				str >> p;
				-- _count;
			}
			success = _checkStreamFail(str);

			_setSuccessText(_edit, text, success);
			return success;
		}

		bool checkParseFileName(MyGUI::EditBox* _edit);

	}
}

#endif
