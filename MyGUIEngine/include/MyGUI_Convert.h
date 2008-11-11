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

		namespace implement
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			std::wstring _MyGUIExport win_x_to_wide(const std::string& _source, unsigned int _code);
			std::string _MyGUIExport win_wide_to_x(const std::wstring& _source, unsigned int _code);
			std::string _MyGUIExport win_x_to_x(const std::string& _source, unsigned int _from_code, unsigned int _to_code);
#endif
		} // namespace implement

		inline std::wstring ansi_to_wide(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_wide(_source, CP_ACP);
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#endif
		}

		inline std::string ansi_to_utf8(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_x(_source, CP_ACP, CP_UTF8);
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#endif
		}

		inline std::wstring utf8_to_wide(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_wide(_source, CP_UTF8);
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#endif
		}

		inline std::string utf8_to_ansi(const std::string& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_x_to_x(_source, CP_UTF8, CP_ACP);
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#endif
		}

		inline std::string wide_to_ansi(const std::wstring& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_wide_to_x(_source, CP_ACP);
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#endif
		}

		inline std::string wide_to_utf8(const std::wstring& _source)
		{
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
			return implement::win_wide_to_x(_source, CP_UTF8);
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_LINUX
#elif MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
#endif
		}


	} // namespace convert

} // namespace MyGUI

#endif // __MYGUI_CONVERT_H__
