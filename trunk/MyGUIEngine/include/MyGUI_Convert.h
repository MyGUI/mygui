/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_CONVERT_H__
#define __MYGUI_CONVERT_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	namespace convert
	{

		std::wstring MYGUI_EXPORT ansi_to_wide(const std::string& _source);
		std::string MYGUI_EXPORT ansi_to_utf8(const std::string& _source);
		std::string MYGUI_EXPORT ansi_to_oem(const std::string& _source);

		std::wstring MYGUI_EXPORT utf8_to_wide(const std::string& _source);
		std::string MYGUI_EXPORT utf8_to_ansi(const std::string& _source);
		std::string MYGUI_EXPORT utf8_to_oem(const std::string& _source);

		std::string MYGUI_EXPORT wide_to_ansi(const std::wstring& _source);
		std::string MYGUI_EXPORT wide_to_utf8(const std::wstring& _source);
		std::string MYGUI_EXPORT wide_to_oem(const std::wstring& _source);

	} // namespace convert

} // namespace MyGUI

#endif // __MYGUI_CONVERT_H__
