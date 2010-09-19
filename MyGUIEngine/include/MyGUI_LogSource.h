/*!
	@file
	@author		Albert Semenov
	@date		04/2010
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
#ifndef __MYGUI_LOG_SOURCE_H__
#define __MYGUI_LOG_SOURCE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ILogFilter.h"
#include "MyGUI_ILogListener.h"
#include <vector>

namespace MyGUI
{

	class MYGUI_EXPORT LogSource
	{
	public:
		LogSource();
		~LogSource();

		/** Set log messages filter. */
		void setLogFilter(ILogFilter* _filter);
		/** Set log messages filter. */
		void addLogListener(ILogListener* _lestener);

		/** Call ILogListener::open() for all listeners. */
		void open();
		/** Call ILogListener::close() for all listeners. */
		void close();
		/** Call ILogListener::flush() for all listeners. */
		void flush();
		/** Call ILogListener::log for all listeners for messages that match LogSource filter (@see void setLogFilter(ILogFilter* _filter)). */
		void log(const std::string& _section, LogLevel _level, const struct tm* _time, const std::string& _message, const char* _file, int _line);

	private:
		typedef std::vector<ILogListener*> VectorLogListeners;
		VectorLogListeners mListeners;
		ILogFilter* mFilter;
	};

} // namespace MyGUI

#endif // __MYGUI_LOG_SOURCE_H__
