/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_COMMON_H__
#define __MYGUI_COMMON_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_Instance.h"
#include "MyGUI_Types.h"
#include "MyGUI_RenderOut.h"
#include "MyGUI_AlignInfo.h"
#include "MyGUI_ResourcePath.h"
#include "OgreException.h"
#include "utility.h"

namespace MyGUI
{

	#define MYGUI_LOG_SECTION "General"
	#define MYGUI_LOG_FILENAME "MyGUI.log"
	#define MYGUI_LOG(level, text) LOGGING(MYGUI_LOG_SECTION, level, text)

	#define MYGUI_EXCEPT(dest) \
	{ \
		MYGUI_LOG(Critical, dest); \
		std::ostringstream stream; \
		stream << dest << "\n" << __FILE__ << "  ( " << __LINE__ << " )"; \
		OGRE_EXCEPT(0, stream.str(), ""); \
	}

	#define MYGUI_ASSERT(exp, dest) \
	{ \
		if ( ! (exp) ) { \
			MYGUI_LOG(Critical, dest); \
			std::ostringstream stream; \
			stream << dest << "\n" << __FILE__ << "  ( " << __LINE__ << " )"; \
			OGRE_EXCEPT(0, stream.str(), ""); \
		} \
	}

	#if MYGUI_DEBUG_MODE == 1
		#define MYGUI_TYPE(type, ptr) MYGUI_ASSERT(null != dynamic_cast<type>(ptr), "failed dynamic cast type")
		#define MYGUI_REGISTER_VALUE(map, value) \
		{ \
			MYGUI_LOG(Info, "Register value : '" << #value << "' = " << value); \
			map[#value] = value; \
		}
		#define MYGUI_DEBUG_ASSERT(exp, dest) MYGUI_ASSERT(exp, dest)
	#else // MYGUI_DEBUG_MODE == 1
		#define MYGUI_TYPE(type, ptr) ((void)0)
		#define MYGUI_REGISTER_VALUE(map, value) map[#value] = value;
		#define MYGUI_DEBUG_ASSERT(exp, dest) ((void)0)
	#endif // MYGUI_DEBUG_MODE == 1

} // namespace MyGUI


#endif // __MYGUI_COMMON_H__
