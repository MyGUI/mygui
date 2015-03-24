/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_LEVEL_LOG_FILTER_H_
#define MYGUI_LEVEL_LOG_FILTER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ILogFilter.h"

namespace MyGUI
{

	class MYGUI_EXPORT LevelLogFilter :
		public ILogFilter
	{
	public:
		LevelLogFilter();
		virtual ~LevelLogFilter();

		//! @copydoc ILogFilter::shouldLog(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line)
		virtual bool shouldLog(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line);

		/** Set logging level.
			@param _value messages with this or higher level will be logged.
		*/
		void setLoggingLevel(LogLevel _value);
		/** Get logging level.
			@return Messages with this or higher level are logged.
		*/
		LogLevel getLoggingLevel() const;

	private:
		LogLevel mLevel;
	};

} // namespace MyGUI

#endif // MYGUI_LEVEL_LOG_FILTER_H_
