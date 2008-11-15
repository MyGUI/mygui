/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __MYGUI_CONVERT_H__
#define __MYGUI_CONVERT_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	namespace convert
	{

		std::wstring _MyGUIExport ansi_to_wide(const std::string& _source);
		std::string _MyGUIExport ansi_to_utf8(const std::string& _source);

		std::wstring _MyGUIExport utf8_to_wide(const std::string& _source);
		std::string _MyGUIExport utf8_to_ansi(const std::string& _source);

		std::string _MyGUIExport wide_to_ansi(const std::wstring& _source);
		std::string _MyGUIExport wide_to_utf8(const std::wstring& _source);

	} // namespace convert

} // namespace MyGUI

#endif // __MYGUI_CONVERT_H__
