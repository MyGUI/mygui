/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef _5544f037_5fc1_43fd_bdf7_5d4c62f2aa36_
#define _5544f037_5fc1_43fd_bdf7_5d4c62f2aa36_

#include "MyGUI_Prerequest.h"

#define MYGUI_PLATFORM_LOG_SECTION "Platform"
#define MYGUI_PLATFORM_LOG_FILENAME "MyGUI.log"
#define MYGUI_PLATFORM_LOG(level, text) MYGUI_LOGGING(MYGUI_PLATFORM_LOG_SECTION, level, text)

#define MYGUI_PLATFORM_EXCEPT(dest) \
do { \
	MYGUI_PLATFORM_LOG(Critical, dest); \
	std::ostringstream stream; \
	stream << dest << "\n"; \
	MYGUI_BASE_EXCEPT(stream.str(), "MyGUI"); \
} while (false)

#define MYGUI_PLATFORM_ASSERT(exp, dest) \
do { \
	if ( ! (exp) ) \
	{ \
		MYGUI_PLATFORM_LOG(Critical, dest); \
		std::ostringstream stream; \
		stream << dest << "\n"; \
		MYGUI_BASE_EXCEPT(stream.str(), "MyGUI"); \
	} \
} while (false)

#endif
