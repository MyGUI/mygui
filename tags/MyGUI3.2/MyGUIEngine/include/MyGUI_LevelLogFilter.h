/*!
	@file
	@author		Albert Semenov
	@date		05/2010
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
#ifndef __MYGUI_LEVEL_LOG_FILTER_H__
#define __MYGUI_LEVEL_LOG_FILTER_H__

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

#endif // __MYGUI_LEVEL_LOG_FILTER_H__
