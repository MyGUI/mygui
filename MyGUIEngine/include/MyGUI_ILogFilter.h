/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_LOG_FILTER_H_
#define MYGUI_I_LOG_FILTER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_LogLevel.h"

namespace MyGUI
{

	class MYGUI_EXPORT ILogFilter
	{
	public:
		virtual ~ILogFilter() { }

		/** Check if specific log message should be logged.\n
			For example some log listeners use only messages with high log level.
		*/
		virtual bool shouldLog(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line)
		{
			return true;
		}
	};

} // namespace MyGUI

#endif // MYGUI_I_LOG_FILTER_H_
