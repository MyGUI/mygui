/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_DIAGNOSTIC_H_
#define MYGUI_DIAGNOSTIC_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Exception.h"
#include "MyGUI_LogManager.h"
#include <sstream>

#define MYGUI_LOG_SECTION "Core"
#define MYGUI_LOG_FILENAME "MyGUI.log"
#define MYGUI_LOG(level, text) MYGUI_LOGGING(MYGUI_LOG_SECTION, level, text)

#define MYGUI_BASE_EXCEPT(desc, src) throw MyGUI::Exception(desc, src, __FILE__, __LINE__)

#define MYGUI_EXCEPT(dest) \
	do \
	{ \
		MYGUI_LOG(Critical, dest); \
		std::ostringstream stream; \
		stream << dest << "\n"; \
		MYGUI_BASE_EXCEPT(stream.str(), "MyGUI"); \
	} while (false)

// NOLINTBEGIN(readability-simplify-boolean-expr)
#define MYGUI_ASSERT(exp, dest) \
	do \
	{ \
		if (!(exp)) \
		{ \
			MYGUI_LOG(Critical, dest); \
			std::ostringstream stream; \
			stream << dest << "\n"; \
			MYGUI_BASE_EXCEPT(stream.str(), "MyGUI"); \
		} \
	} while (false)
// NOLINTEND(readability-simplify-boolean-expr)

#define MYGUI_ASSERT_RANGE(index, size, owner) \
	MYGUI_ASSERT(index < size, owner << " : index number " << index << " out of range [" << size << "]")
#define MYGUI_ASSERT_RANGE_AND_NONE(index, size, owner) \
	MYGUI_ASSERT( \
		index < size || index == MyGUI::ITEM_NONE, \
		owner << " : index number " << index << " out of range [" << size << "]")
#define MYGUI_ASSERT_RANGE_INSERT(index, size, owner) \
	MYGUI_ASSERT( \
		(index <= size) || (index == MyGUI::ITEM_NONE), \
		owner << " : insert index number " << index << " out of range [" << size << "] or not ITEM_NONE")

#if MYGUI_DEBUG_MODE == 1
	#define MYGUI_REGISTER_VALUE(map, value) \
		do \
		{ \
			MYGUI_LOG(Info, "Register value : '" << #value << "' = " << (int)value); \
			map[#value] = value; \
		} while (false)
	#define MYGUI_DEBUG_ASSERT(exp, dest) MYGUI_ASSERT(exp, dest)
#else
	#define MYGUI_REGISTER_VALUE(map, value) map[#value] = value
	#define MYGUI_DEBUG_ASSERT(exp, dest) ((void)0)
#endif

#define MYGUI_OBSOLETE(text) /*! \deprecated text */ [[deprecated(text)]]

#endif // MYGUI_DIAGNOSTIC_H_
