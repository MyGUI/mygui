/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __MYGUI_CONVERT_H__
#define __MYGUI_CONVERT_H__

#include "MyGUI_Prerequest.h"

#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
#include "windows.h"
#endif

namespace MyGUI
{

	namespace convert
	{

		inline std::wstring _MyGUIExport ansi_to_wide(const std::string& _source);

		inline std::string _MyGUIExport ansi_to_utf8(const std::string& _source);

		inline std::wstring _MyGUIExport utf8_to_wide(const std::string& _source);

		inline std::string _MyGUIExport utf8_to_ansi(const std::string& _source);

		inline std::string _MyGUIExport wide_to_ansi(const std::wstring& _source);

		inline std::string _MyGUIExport wide_to_utf8(const std::wstring& _source);

	} // namespace convert

} // namespace MyGUI

#endif // __MYGUI_CONVERT_H__
