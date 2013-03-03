/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_COMMON_H__
#define __MYGUI_COMMON_H__

#include "MyGUI_Prerequest.h"

#include <string>
#include <list>
#include <set>
#include <map>
#include <vector>
#include <deque>
#include <exception>
#include <math.h>

#ifdef MYGUI_CUSTOM_ALLOCATOR
#    include "MyGUI_CustomAllocator.h"
#else // MYGUI_CUSTOM_ALLOCATOR
#    include "MyGUI_Allocator.h"
#endif // MYGUI_CUSTOM_ALLOCATOR

// этот дефайн для того чтобы в самом гуе показывалось имя файла где вызывается new
#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
#	ifdef MYGUI_CHECK_MEMORY_LEAKS
#		define DEBUG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#		define new DEBUG_NEW
#	endif
#endif

#include "MyGUI_Macros.h"
#include "MyGUI_Diagnostic.h"
#include "MyGUI_LogManager.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_Types.h"
#include "MyGUI_StringUtility.h"
#include "MyGUI_MouseButton.h"
#include "MyGUI_KeyCode.h"
#include "MyGUI_Version.h"
#include "MyGUI_WidgetStyle.h"
#include "MyGUI_UString.h"
#include "MyGUI_Delegate.h"

#endif // __MYGUI_COMMON_H__
