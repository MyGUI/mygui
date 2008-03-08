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
#include <OgreException.h>
#include "MyGUI_Utility.h"

namespace MyGUI
{
	class _MyGUIExport MyGUIException : public Ogre::Exception
	{
	public:
		MyGUIException(int number, const Ogre::String& description, const Ogre::String& source, const char* file, long line)
			: Exception(number, description, source, "MyGUIException", file, line) {}
		//virtual ~MyGUIException() {}
	};

	// just other number
	#define ERR_MY_GUI Ogre::Exception::ERR_NOT_IMPLEMENTED+1
	static MyGUIException create(
			Ogre::ExceptionCodeType<ERR_MY_GUI> code,
			const Ogre::String& desc,
			const Ogre::String& src, const char* file, long line)
		{
			return MyGUIException(code.number, desc, src, file, line);
		}



	// copy of OGRE_EXCEPT with MyGUIException create
	#define OGRE_BASED_EXCEPT(desc, src)	throw MyGUI::create(Ogre::ExceptionCodeType<ERR_MY_GUI>(), desc, src, __FILE__, __LINE__ );

	#define MYGUI_LOG_SECTION "General"
	#define MYGUI_LOG_FILENAME "MyGUI.log"
	#define MYGUI_LOG(level, text) LOGGING(MYGUI_LOG_SECTION, level, text)

	#define MYGUI_EXCEPT(dest) \
	{ \
		MYGUI_LOG(Critical, dest); \
		std::ostringstream stream; \
		stream << dest << "\n"; \
		OGRE_BASED_EXCEPT(stream.str(), "MyGUI"); \
	}

	#define MYGUI_ASSERT(exp, dest) \
	{ \
		if ( ! (exp) ) { \
			MYGUI_LOG(Critical, dest); \
			std::ostringstream stream; \
			stream << dest << "\n"; \
			OGRE_BASED_EXCEPT(stream.str(), "MyGUI"); \
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


	#define MYGUI_DEFAULT_CHARSET "Unicode"

	#if OGRE_UNICODE_SUPPORT
		#define _T(_str) L##_str
		#define MYGUI_CHARSET "Unicode"
		#define MYGUI_UNICODE_SUPPORT 1
		#define MYGUI_CHARSET_LIMIT 65535
		#define MYGUI_DEREF_DISPLAYSTRING_ITERATOR(it) it.getCharacter()
		typedef wchar_t Char;
	#else
		#define _T(_str) _str
		#define MYGUI_CHARSET "Multi-Byte"
		#define MYGUI_UNICODE_SUPPORT 0
		#define MYGUI_CHARSET_LIMIT 255
		#define MYGUI_DEREF_DISPLAYSTRING_ITERATOR(it) ((unsigned char)*it)
		typedef char Char;
	#endif

} // namespace MyGUI


#endif // __MYGUI_COMMON_H__
