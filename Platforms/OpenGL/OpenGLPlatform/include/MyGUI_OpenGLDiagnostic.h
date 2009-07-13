/*!
	@file
	@author		George Evmenov
	@date		07/2009
	@module
*/
/*
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
#ifndef __MYGUI_OPENGL_DIAGNOSTIC_H__
#define __MYGUI_OPENGL_DIAGNOSTIC_H__

#include "MyGUI_Prerequest.h"

#define MYGUI_PLATFORM_LOG_SECTION "Platform"
#define MYGUI_PLATFORM_LOG_FILENAME "MyGUI.log"
#define MYGUI_PLATFORM_LOG(level, text) MYGUI_LOGGING(MYGUI_PLATFORM_LOG_SECTION, level, text)

#define MYGUI_PLATFORM_EXCEPT(dest) \
{ \
	MYGUI_PLATFORM_LOG(Critical, dest); \
	MYGUI_DBG_BREAK;\
	std::ostringstream stream; \
	stream << dest << "\n"; \
	MYGUI_BASE_EXCEPT(stream.str().c_str(), "MyGUI"); \
}

#define MYGUI_PLATFORM_ASSERT(exp, dest) \
{ \
	if ( ! (exp) ) \
	{ \
		MYGUI_PLATFORM_LOG(Critical, dest); \
		MYGUI_DBG_BREAK;\
		std::ostringstream stream; \
		stream << dest << "\n"; \
		MYGUI_BASE_EXCEPT(stream.str().c_str(), "MyGUI"); \
	} \
}

#endif // __MYGUI_OPENGL_DIAGNOSTIC_H__
