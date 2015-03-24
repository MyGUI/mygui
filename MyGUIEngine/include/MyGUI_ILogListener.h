/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_LOG_LISTENER_H_
#define MYGUI_I_LOG_LISTENER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_LogLevel.h"

namespace MyGUI
{

	class MYGUI_EXPORT ILogListener
	{
	public:
		virtual ~ILogListener() { }

		/** Opens log listener. (for example open file) */
		virtual void open() { }
		/** Opens log listener. (for example close file) */
		virtual void close() { }
		/** Flush log listener messages.*/
		virtual void flush() { }
		/** Log message.*/
		virtual void log(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line) { }
	};

} // namespace MyGUI

#endif // MYGUI_I_LOG_LISTENER_H_
