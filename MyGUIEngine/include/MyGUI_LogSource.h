/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_LOG_SOURCE_H_
#define MYGUI_LOG_SOURCE_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ILogFilter.h"
#include "MyGUI_ILogListener.h"
#include <vector>

namespace MyGUI
{

	class MYGUI_EXPORT LogSource
	{
	public:
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
		void log(
			std::string_view _section,
			LogLevel _level,
			const struct tm* _time,
			std::string_view _message,
			std::string_view _file,
			int _line);

	private:
		using VectorLogListeners = std::vector<ILogListener*>;
		VectorLogListeners mListeners;
		ILogFilter* mFilter{nullptr};
	};

} // namespace MyGUI

#endif // MYGUI_LOG_SOURCE_H_
